#include "SynthVoice.h"

// Check that Synthesiser Sound is casting correctly. If so we can play a sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	osc.setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	gain.setGainLinear(velocity);
	vcaADSR.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	vcaADSR.noteOff();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	osc.prepare(spec);
	gain.prepare(spec);

	gain.setGainLinear(0.1f);			// We'll want a slider for this -p
	vcaADSR.setSampleRate(sampleRate);
	
	isPrepared = true;
}

void SynthVoice::renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);			// Stop the project if prepareToPlay has not been called -p

	dsp::AudioBlock<float> audioBlock{ outputBuffer };
	osc.process(dsp::ProcessContextReplacing<float>(audioBlock));
	gain.process(dsp::ProcessContextReplacing<float>(audioBlock));
	vcaADSR.applyEnvelopeToBuffer(outputBuffer, 0, numSamples);
}
