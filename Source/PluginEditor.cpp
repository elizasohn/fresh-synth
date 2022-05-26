/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FreshSynthAudioProcessorEditor::FreshSynthAudioProcessorEditor (FreshSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    mEditorLogger.setText(p.mEditorLogText);
    mEditorLogger.setColour(juce::Colours::white);
    //mEditorLogger.setBounds(0, 0, 200, 20);
    //mEditorLogger.setFontHeight(15.0f);
    addAndMakeVisible(mEditorLogger);

    setSize(960, 540);
}

FreshSynthAudioProcessorEditor::~FreshSynthAudioProcessorEditor()
{
}

//==============================================================================
// This is kind of like a screen update/refresh
void FreshSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Now, this is a story all about how my life got flipped-turned upside down", getLocalBounds(), juce::Justification::centred, 1);
}

// Where you actually lay out the visual elements of the plugin
void FreshSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
