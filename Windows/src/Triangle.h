#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TriangleWaveSound : public SynthesiserSound
{
public:
	TriangleWaveSound() {}

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

class TriangleWaveVoice : public SynthesiserVoice
{
public:
	TriangleWaveVoice()
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<TriangleWaveSound*> (sound) != nullptr;
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

	double getCurrentAngle()
	{
		return currentAngle;
	}

	double getAngleDelta()
	{
		return angleDelta;
	}

	double getLevel()
	{
		return level;
	}

	double getTailOff()
	{
		return tailOff;
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
					auto currentSample = static_cast<FloatType> (triangleSample(currentAngle, 1, level) * tailOff);

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
					auto currentSample = static_cast<FloatType> (triangleSample(currentAngle, 1, level));

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

double triangleSample(double t, double frequency, double level) {
    double res = 0.0;
    double fullPeriodTime = 1.0 / frequency;
    double localTime = fmod(t, fullPeriodTime);
    
    double value = localTime / fullPeriodTime;
    
    if (value < 0.25) {
        res = value * 4;
    }
    else if (value < 0.75) {
        res = 2.0 - (value * 4.0);
    }
    else {
        res = value * 4 - 4.0;
    }
    
    return level * res;
}
