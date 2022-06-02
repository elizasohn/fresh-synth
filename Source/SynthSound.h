#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
	// These were originally declared in juce synthesiser sound - hence override
	bool appliesToNote(int midiNoteNumber) override { return true; }
	bool appliesToChannel(int midiChannel) override { return true; }
};
