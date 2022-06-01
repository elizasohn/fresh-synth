#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public SynthesiserVoice
{
public:
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
private:
	dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); }};
	dsp::Gain<float> gain;
	ADSR vcaADSR;
	ADSR::Parameters vcaADSRParams;

	bool isPrepared = false;
};