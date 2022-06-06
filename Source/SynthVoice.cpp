#include "SynthVoice.h"

// Check that Synthesiser Sound is casting correctly. If so we can play a sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), true);
<<<<<<< HEAD
	// Doesn't work - I suspect because we are updating the gain repeatedly along with other params
	// Need to set up a listener.
	//juce::Logger::outputDebugString(juce::String(velocity));
	//float oldGain = gain.getGainLinear();
	//float newGain = (velocity * velocityAmt) + oldGain;
	//gain.setGainLinear(newGain);
=======
	gain.setGainLinear(velocity);		// currently clips like crazy -p
>>>>>>> 50c5b26c5acb378353385dedfb4989e5dc42e530
	vcaADSR.noteOn();
    filterADSR.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	// Note: Sometimes the release holds when the sustain is set to 0.001
	vcaADSR.noteOff();
    filterADSR.noteOff();

	if (!allowTailOff || !vcaADSR.isActive())
		clearCurrentNote();
}

//void SynthVoice::setLevel(const float gainValue)
//{
////    auto& gain = processorChain.template get<gainIndex>();
////    gain.setGainLinear(gainValue);
//}
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
	filterADSR.setSampleRate(sampleRate);
    vcaADSR.setSampleRate(sampleRate);

	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	osc.prepare(spec);
	gain.prepare(spec);
	filter.prepare(spec);
<<<<<<< HEAD
	velocityAmt = 0.5f;

=======
    
>>>>>>> 50c5b26c5acb378353385dedfb4989e5dc42e530
	gain.setGainLinear(0.7f);
	filter.setMode(juce::dsp::LadderFilterMode::LPF24);
	filter.setCutoffFrequencyHz(20000.0f);
	filter.setEnabled(true);
	filter.setResonance(0.0f);
	
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
	// nice to haves: triangle wave, noise... better sounding oscillators :P
}

void SynthVoice::setGain(const float newGain, const float newVelocity)
{
	gain.setGainLinear(newGain);
	velocityAmt = newVelocity;
}


void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
	vcaADSRParams.attack = attack;
	vcaADSRParams.decay = decay;
	vcaADSRParams.sustain = sustain;
	vcaADSRParams.release = release;

	vcaADSR.setParameters(vcaADSRParams);
}

void SynthVoice::updateFilterADSR(const float fAttack, const float fDecay, const float fSustain, const float fRelease)
{
    filterADSRParams.attack = fAttack;
    filterADSRParams.decay = fDecay;
    filterADSRParams.sustain = fSustain;
    filterADSRParams.release = fRelease;

	filterADSR.setParameters(filterADSRParams);
}

void SynthVoice::updateFilter(const float cutoff, const float resonance, const int filterType)
{
	filter.setMode(juce::dsp::LadderFilterMode(filterType));
	
    filter.setCutoffFrequencyHz(cutoff);
    filter.setResonance(resonance);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);			// This stops the project if prepareToPlay has not been called -p
	jassert(oscReady);

	// if the voice is silent we return
	if (!isVoiceActive())
		return;

	synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, false);
    filterADSR.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);
	synthBuffer.clear();

	juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
	osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
	gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    vcaADSR.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
	filter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
	// go through each channel add the synth buffer to outputBuffer
	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

		// if the ADSR is not active - clear the current note
		if (!vcaADSR.isActive())
			clearCurrentNote();
	}
}
