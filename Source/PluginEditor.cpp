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
    mMidiDisplay.setFont(juce::Font(font_height, juce::Font::plain));
    mMidiDisplay.setText(p.mMidiText, juce::dontSendNotification);
    //mMidiDisplay.setColour(Label::textColourId, Colours::white);
    mMidiDisplay.setBounds(0, 0, 200, font_height);
    addAndMakeVisible(mMidiDisplay);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    
    oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);
    filterSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTER", filterSelector);

    cutoffSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffSlider);
    resonanceSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RESONANCE", resonanceSlider);

    // Gain
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(gainSlider);
    
    // ADSR 
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    attackSlider.setTitle("Attack");
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", dontSendNotification);
    attackLabel.setJustificationType(Justification::centred);
    attackLabel.attachToComponent(&attackSlider, false);
    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(decayLabel);
    decayLabel.setText("Decay", dontSendNotification);
    decayLabel.setJustificationType(Justification::centred);
    decayLabel.attachToComponent(&decaySlider, false);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(sustainLabel);
    sustainLabel.setText("Sustain", dontSendNotification);
    sustainLabel.setJustificationType(Justification::centred);
    sustainLabel.attachToComponent(&sustainSlider, false);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", dontSendNotification);
    releaseLabel.setJustificationType(Justification::centred);
    releaseLabel.attachToComponent(&releaseSlider, false);

    // OSC
    oscSelector.addItemList({ "Sine", "Saw", "Square" }, 1);    
    oscSelector.setSelectedId(1);
    addAndMakeVisible(oscSelector);

    // Filter Controls
    cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    cutoffSlider.setMouseDragSensitivity(500);
    addAndMakeVisible(cutoffSlider);
    
    resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(resonanceSlider);

    filterSelector.addItemList({ "LPF12", "HPF12", "BPF12", "LPF24", "HPF24", "BPF24" }, 1);
    filterSelector.setSelectedId(1);
    addAndMakeVisible(filterSelector);
    
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
//    g.drawFittedText ("Now, this is a story all about how my life got flipped-turned upside down", getLocalBounds(), juce::Justification::centred, 1);
}


void FreshSynthAudioProcessorEditor::resized()
{
    const auto bounds = getLocalBounds().reduced (10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 8 - padding;
    const auto sliderHeight = bounds.getWidth() / 6 - padding;
    const auto halfWidth = bounds.getWidth()/2;
    const auto sliderStartY = bounds.getHeight()/3 - (sliderHeight/2);
    const auto dialWidth = 175;
    const auto dialHeight = 175;

    attackSlider.setBounds (halfWidth, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds (attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds (decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds (sustainSlider.getRight(), sliderStartY, sliderWidth, sliderHeight);

    oscSelector.setBounds(30, sliderStartY, getWidth()/2 - 50, 20);
    filterSelector.setBounds(30, sliderStartY + 50, getWidth()/2 - 50, 20);

    cutoffSlider.setBounds(halfWidth, bounds.getHeight() / 2, dialWidth, dialHeight);
    resonanceSlider.setBounds(cutoffSlider.getRight() + 40, bounds.getHeight() / 2, dialWidth, dialHeight);
}
