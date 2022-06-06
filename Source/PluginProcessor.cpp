/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    This file is where the backend audio processing is done -p

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
FreshSynthAudioProcessor::FreshSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "parameters", createParameters())
#endif
{
    // Get Default MIDI name and display. 
    // Pretty sure we don't need this at all, but it's nice to see the midiDevice selected -p
    juce::MidiDeviceInfo midiDevice = juce::MidiInput::getDefaultDevice();
    this->mMidiText = "MIDI Device: " + midiDevice.name;
    /*
        // A way to grab info on all the available midi devices. Uses JUCE array / MidiDeviceInfo
        Array<MidiDeviceInfo> midiDevices = MidiInput::getAvailableDevices();
    */

    // Note the synthesiser class will automatically delete these allocated sound / voices
    // Note 2: This is currently monophonic
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

FreshSynthAudioProcessor::~FreshSynthAudioProcessor()
{
}

//==============================================================================
const juce::String FreshSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FreshSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FreshSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FreshSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FreshSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FreshSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FreshSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FreshSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FreshSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FreshSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
// Tends to get called when the plugin is in a stop state and you are getting ready to process audio again or for the first time -p
// Good place to clear out any left over values in the processor -p
void FreshSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        // Since our synth is a custom class and inherited from Juce::Synthesiser. We need to cast. -p
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void FreshSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FreshSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// The audio-callback function. This gets called from a high-priority thread. -p
// Dont do here: any logging, memory allocation (results in crackles) -p
void FreshSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        //buffer.clear (i, 0, buffer.getNumSamples());


    // updates the parameter value tree for the synth -p
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // OSC controls
            auto& wave = *apvts.getRawParameterValue("OSC");
            // Gain
//            auto& gain = *apvts.getRawParameterValue("GAIN");
//            voice->setWave(wave);
            
            // Amp ADSR
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            
            // Filter ADSR
            auto& fAttack = *apvts.getRawParameterValue("FATTACK");
            auto& fDecay = *apvts.getRawParameterValue("FDECAY");
            auto& fSustain = *apvts.getRawParameterValue("FSUSTAIN");
            auto& fRelease = *apvts.getRawParameterValue("FRELEASE");
            
            // Filter
            auto& cutoffFreq = *apvts.getRawParameterValue("CUTOFF");
            auto& resonancePeak = *apvts.getRawParameterValue("RESONANCE");
            auto& filterType = *apvts.getRawParameterValue("FILTER");
            
            // Update Voice
            voice->setWave(wave);
<<<<<<< HEAD
            auto& gain = *apvts.getRawParameterValue("GAIN");
            auto& velocity = *apvts.getRawParameterValue("VELOCITY");
            voice->setGain(gain, velocity);
=======
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            voice->updateFilterADSR(fAttack.load(), fDecay.load(), fSustain.load(), fRelease.load());
            voice->updateFilter(cutoffFreq.load(), resonancePeak.load(), filterType.load());
            
            
>>>>>>> 50c5b26c5acb378353385dedfb4989e5dc42e530

            // LFO
            
        }
    }

    /*
    
    for (const juce::MidiMessageMetadata metadata : midiMessages)
        if (metadata.numBytes == 3)
            juce::Logger::writeToLog("TimeStamp: " + juce::String(metadata.getMessage().getTimeStamp()));
    */

    // this calls all of the synthVoice's render next blocks as well
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


    /*
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    */
    
}

//==============================================================================
bool FreshSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FreshSynthAudioProcessor::createEditor()
{
    return new FreshSynthAudioProcessorEditor (*this);
}

//==============================================================================
// If you want to save the state of the plugin and load it up later. That is done here. -p
// getStateInfo saves a snapshot of the plugin at the time -p
void FreshSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

// useStateInformation uses the data from a snapshot and load it into your plugin -p
void FreshSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FreshSynthAudioProcessor();
}

// Synth Parameter Value Tree
juce::AudioProcessorValueTreeState::ParameterLayout FreshSynthAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    
    // Gain
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 1.0f));
    
    // OSC
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VELOCITY", "Velocity Amount", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f, 0.5f, false }, 0.01f));

    // ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.01f, 3.0f, 0.01f, 0.5f, false }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.01f, 4.0f, 0.01f, 0.5f, false }, 0.65f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.01f, 1.2f, false}, 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.01f, 4.0f, 0.01f, 0.3f, false }, 0.05f));

    // FILTER ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FATTACK", "Filter Attack", juce::NormalisableRange<float> { 0.01f, 3.0f, 0.01f, 0.5f, false}, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FDECAY", "Filter Decay", juce::NormalisableRange<float> { 0.01f, 4.0f, 0.01f, 0.5f, false }, 0.65f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FSUSTAIN", "Filter Sustain", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f, 1.2f, false }, 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FRELEASE", "Filter Release", juce::NormalisableRange<float> { 0.01f, 4.0f, 0.01f, 0.3f, false }, 0.05f));

    // FILTER
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Filter Cutoff", juce::NormalisableRange<float> {20.0f, 20000.0f, 0.01f, 0.3f, false}, 20000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RESONANCE", "Resonance Control", juce::NormalisableRange<float> {0.0f, 1.0f, 0.001f, 1.0f, false}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTER", "Filter Type", juce::StringArray{ "LPF12", "HPF12", "BPF12", "LPF24", "HPF24", "BPF24"}, 0));

    return { params.begin(), params.end() };
}
