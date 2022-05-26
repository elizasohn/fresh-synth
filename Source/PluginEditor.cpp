/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
    This file is where the front-end interface is created -p

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor for the editor window -p
FreshSynthAudioProcessorEditor::FreshSynthAudioProcessorEditor (FreshSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    float font_height = 10.0f;
    mMidiDisplay.setFont(Font(font_height, Font::plain));
    mMidiDisplay.setText(p.mMidiText, dontSendNotification);
    //mMidiDisplay.setColour(Label::textColourId, Colours::white);
    mMidiDisplay.setBounds(0, 0, 200, font_height);
    addAndMakeVisible(mMidiDisplay);

    mEditorLogger.setFont(Font(font_height, Font::plain));
    mEditorLogger.setText("Waiting", dontSendNotification);
    mEditorLogger.setBounds(0, 10, 200, font_height);
    addAndMakeVisible(mEditorLogger);

    setSize(960, 540);
}

FreshSynthAudioProcessorEditor::~FreshSynthAudioProcessorEditor()
{
}

//==============================================================================
// This is kind of like a screen update/refresh -p
void FreshSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Now, this is a story all about how my life got flipped-turned upside down", getLocalBounds(), juce::Justification::centred, 1);
}

// Where you actually lay out the visual elements of the plugin -p
void FreshSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

}
