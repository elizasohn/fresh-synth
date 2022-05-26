/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    This file is where the backend audio processing is done -p

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
                       )
#endif
{
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
    
    // Set up MIDI in using the default midi device. Stretch goal: Choosable MIDI device menu
    MidiDeviceInfo midiDevice = MidiInput::getDefaultDevice();
    this->mEditorLogText = midiDevice.name;
    //this->mEditorLogText = "THIS IS A REALLY LONG STRING LET'S SEE WHAT IT DOES WHOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO YEAH";
    /* 
        // A way to grab info on all the available midi devices. Uses JUCE array / MidiDeviceInfo
        Array<MidiDeviceInfo> midiDevices = MidiInput::getAvailableDevices();
        if (midiDevices.isEmpty())
            Logger::outputDebugString("NO MIDI FOUNNNNNNNNNND");
        else
            Logger::outputDebugString("MIDI FOUNNNNNNNNNND");
    */
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
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

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
