/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    This file is where the backend audio processing is done -p

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class FreshSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FreshSynthAudioProcessor();
    ~FreshSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    
    // OUR VARIABLES ===============================================================
    juce::String mMidiText = "Waiting...";
    // ADSR Ranges
    // These had to be assigned in PluginProcessor. For reference by ADSR sliders
    float attackStart;
    float attackEnd;
    float attackDefault;
    float decayStart;
    float decayEnd;
    float decayDefault;
    float sustainStart;
    float sustainEnd;
    float sustainDefault;
    float releaseStart;
    float releaseEnd;
    float releaseDefault;

    // Filter ADSR Ranges
    float fAttackStart;
    float fAttackEnd;
    float fAttackDefault;
    float fDecayStart;
    float fDecayEnd;
    float fDecayDefault;
    float fSustainStart;
    float fSustainEnd;
    float fSustainDefault;
    float fReleaseStart;
    float fReleaseEnd;
    float fReleaseDefault;

    // Filter Controls
    float cutoffStart;
    float cutoffEnd;
    float resonanceStart;
    float resonanceEnd;



private:
    // Declare our synth object
    juce::Synthesiser synth;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreshSynthAudioProcessor)
};
