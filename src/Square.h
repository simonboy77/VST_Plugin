/*
==============================================================================

Square.h
Created: 21 Oct 2017 5:41:57pm
Author:  Simon

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SquareWaveSound : public SynthesiserSound
{
public:
	SquareWaveSound() {}

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

class SquareWaveVoice : public SynthesiserVoice
{
public:
	SquareWaveVoice()
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SquareWaveSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/) override
	{
		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;

		double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * double_Pi;
	}

	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			// start a tail-off by setting this flag. The render callback will pick up on
			// this and do a fade out, calling clearCurrentNote() when it's finished.

			if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
								// stopNote method could be called more than once.
				tailOff = 1.0;
		}
		else
		{
			// we're being told to stop playing immediately, so reset everything..

			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int /*newValue*/) override
	{
		// for override
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// for override
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

	void renderNextBlock(AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

private:
	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{
		if (angleDelta != 0.0)
		{
			if (tailOff > 0)
			{
				while (--numSamples >= 0)
				{
					//auto currentSample = static_cast<FloatType> (std::sin(currentAngle) * level * tailOff);
					auto currentSample = static_cast<FloatType> (sign(std::sin(currentAngle) * level * tailOff));

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;

					tailOff *= 0.99;

					if (tailOff <= 0.005)
					{
						clearCurrentNote();

						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0)
				{
					//auto currentSample = static_cast<FloatType> (std::sin(currentAngle) * level);
					auto currentSample = static_cast<FloatType> (sign(std::sin(currentAngle) * level));

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}

	double currentAngle = 0, angleDelta = 0, level = 0, tailOff = 0;
};

double sign(double val)
{
	return (val >= 0.0) ? 0.1 : -0.1;
}

