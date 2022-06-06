#include "SynthVoice.h"

// Check that Synthesiser Sound is casting correctly. If so we can play a sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	// Note: Currently there seems to be a rather annoying portamento. Might have something to do with this code
	osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	//gain.setGainLinear(velocity);		// currently clips like crazy -p
	vcaADSR.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	// Note: Sometimes the release holds even when set to 0.01 and I don't know why -p
	vcaADSR.noteOff();

	if (!allowTailOff || !vcaADSR.isActive())
		clearCurrentNote();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	//juce::Logger::writeToLog(juce::String(newPitchWheelValue));
	//osc.setFrequency(osc.getFrequency() + newPitchWheelValue);
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	vcaADSR.setSampleRate(sampleRate);

	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	osc.prepare(spec);
	gain.prepare(spec);

	gain.setGainLinear(0.7f);
	
	isPrepared = true;
}


void SynthVoice::setWave(const int waveType)
{
	switch (waveType)
	{
		// Sine wave
		case 0:
			osc.initialise([](float x) { return std::sin(x); });
			break;
		// Saw wave
		case 1:
			osc.initialise([](float x) {return x / juce::MathConstants<float>::pi; });
			break;
		// Square wave
		case 2:
			osc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
			break;
		default:
			jassertfalse;
	}
	oscReady = true;
	// nice to haves: triangle wave, noise
}


void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
	vcaADSRParams.attack = attack;
	vcaADSRParams.decay = decay;
	vcaADSRParams.sustain = sustain;
	vcaADSRParams.release = release;

	vcaADSR.setParameters(vcaADSRParams);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);			// This stops the project if prepareToPlay has not been called -p
	jassert(oscReady);

	// if the voice is silent we return
	if (!isVoiceActive())
		return;

	synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, false);
	synthBuffer.clear();

	juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
	osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
	gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
	vcaADSR.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

	// go through each channel add the synth buffer to outputBuffer
	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

		// if the ADSR is not active - clear the current note
		if (!vcaADSR.isActive())
			clearCurrentNote();
	}


}
