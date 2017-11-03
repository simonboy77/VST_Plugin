/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class SimonVstpluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SimonVstpluginAudioProcessor();
    ~SimonVstpluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
	{
		jassert(!isUsingDoublePrecision());
		process(buffer, midiMessages, delayBufferFloat);
	}

	void processBlock(AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
	{
		jassert(isUsingDoublePrecision());
		process(buffer, midiMessages, delayBufferDouble);
	}

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
	void setVoiceAmount(int amount);
	int getCurWave();
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void initialiseSineSynth();
	void initialiseSquareSynth();
	void initialiseTriangleSynth();
	void initialiseSawSynth();

	MidiKeyboardState keyboardState;

	int lastUIWidth = 400, lastUIHeight = 200;
	
private:
    //==============================================================================
	template <typename FloatType>
	void process(AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, AudioBuffer<FloatType>& delayBuffer);
	void ResetSynth();
	
	AudioBuffer<float> delayBufferFloat;
	AudioBuffer<double> delayBufferDouble;

	Synthesiser synth;

	static BusesProperties getBusesProperties();

	int voiceAmount;
	int curWave; // 0=sine, 1=square, 2=triangle, 3=saw

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimonVstpluginAudioProcessor)
};
