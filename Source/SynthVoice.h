#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void setWave(const int waveType);
	void setGain(const float newGain, const float newVelocity);
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
	void updateFilterADSR(const float attack, const float decay, const float sustain, const float release);
	void updateFilter(const float cutoff, const float resonance, const int filterType);
	void modFilter(const float cutoff, const float mod);
	void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
private:
	//juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };
	juce::dsp::Oscillator<float> osc;
	juce::dsp::Gain<float> gain;
	juce::ADSR vcaADSR;
	juce::ADSR::Parameters vcaADSRParams;
	juce::AudioBuffer<float> synthBuffer;

	juce::dsp::LadderFilter<float> filter;
	juce::ADSR filterADSR;
	juce::ADSR::Parameters filterADSRParams;

	bool oscReady = false;
	bool isPrepared = false;
	float velocityAmt;
};
