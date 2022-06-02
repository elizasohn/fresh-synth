#include "SynthVoice.h"

// Check that Synthesiser Sound is casting correctly. If so we can play a sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	osc.setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	//gain.setGainLinear(velocity);		// currently clips like crazy -p
	vcaADSR.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	vcaADSR.noteOff();

	//if (!allowTailOff || !vcaADSR.isActive())
		//clearCurrentNote();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	vcaADSR.setSampleRate(sampleRate);

	dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	osc.prepare(spec);
	gain.prepare(spec);

	gain.setGainLinear(0.7f);			// We'll want a slider for this -p
	vcaADSRParams.attack = 0.8f;
	//vcaADSRParams.decay = 0.8f;
	//vcaADSRParams.sustain = 1.0f;
	vcaADSRParams.release = 1.5f;
	vcaADSR.setParameters(vcaADSRParams);
	
	isPrepared = true;
}

void SynthVoice::renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);			// Stop the project if prepareToPlay has not been called -p

	/*
	// potential fix for midi problems
	jassert(isPrepared);
	if (!isVoiceActive())
		return;

	synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	synthBuffer.clear();

	juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
	osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
	gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

	vcaADSR.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

	if (startSample != 0)
		//jassertfalse;

	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

		if (!vcaADSR.isActive())
			clearCurrentNote();
	}
	*/
	

	dsp::AudioBlock<float> audioBlock{ outputBuffer };
	osc.process(dsp::ProcessContextReplacing<float>(audioBlock));
	gain.process(dsp::ProcessContextReplacing<float>(audioBlock));

	vcaADSR.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
	
}
