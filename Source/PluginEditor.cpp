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

    // mEditorLogger.setBoundingBox(Parallelogram<float>(Point<float>(0.0f, 0.0f), Point<float>(1.5f, 0.0f), Point<float>(0.0f, -1.5f)));
    // mEditorLogger.setText(p.mEditorLogText);
    // mEditorLogger.setColour(juce::Colours::white);
    // mEditorLogger.setFontHeight(15.0f);
    // mEditorLogger.setFontHorizontalScale(2.0f);
    // addAndMakeVisible(mEditorLogger);

    //testLabel.setFont(Font(16.0f, Font::bold));
    testLabel.setText(p.mEditorLogText, dontSendNotification);
    //testLabel.setColour(Label::textColourId, Colours::white);
    //testLabel.setJustificationType(Justification::centred);
    testLabel.setBounds(0, 0, 200, 20);

    addAndMakeVisible(testLabel);


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
