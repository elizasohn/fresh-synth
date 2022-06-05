#include "SynthVoice.h"

// Check that Synthesiser Sound is casting correctly. If so we can play a sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	//gain.setGainLinear(velocity);		// currently clips like crazy -p
	vcaADSR.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
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
	vcaADSRParams.attack = 0.8f;
	vcaADSRParams.decay = 0.8f;
	vcaADSRParams.sustain = 1.0f;
	vcaADSRParams.release = 1.5f;
	vcaADSR.setParameters(vcaADSRParams);
	
	isPrepared = true;
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
