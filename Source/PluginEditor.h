/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
    This file is where the front-end interface is created -p

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FreshSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FreshSynthAudioProcessorEditor (FreshSynthAudioProcessor&);
    ~FreshSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it. Can pass information from the backend to front
    FreshSynthAudioProcessor& audioProcessor;


    // OUR VARIABLES ===============================================================
    juce::Label mMidiDisplay;
    // gain
    juce::Slider gainSlider;
    // asdr
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    // osc
    juce::ComboBox oscSelector;
    juce::ComboBox filterSelector;
    // filter
    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;

    // Selectors
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterSelAttachment;

    // Sliders
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreshSynthAudioProcessorEditor)
};
