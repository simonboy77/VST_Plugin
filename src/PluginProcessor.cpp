/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Sine.h"
#include "Square.h"
#include "Triangle.h"
#include "Saw.h"

//==============================================================================

SimonVstpluginAudioProcessor::SimonVstpluginAudioProcessor()
     : AudioProcessor (getBusesProperties())
{
	voiceAmount = 8;
	
	initialiseSineSynth();
}

SimonVstpluginAudioProcessor::~SimonVstpluginAudioProcessor()
{
}

void SimonVstpluginAudioProcessor::ResetSynth()
{
	//synth.allNotesOff();
	synth.clearSounds();
	synth.clearVoices();
}

void SimonVstpluginAudioProcessor::initialiseSineSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new SineWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new SineWaveSound());

	curWave = 0;
}

void SimonVstpluginAudioProcessor::initialiseSquareSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new SquareWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new SquareWaveSound());

	curWave = 1;
}

void SimonVstpluginAudioProcessor::initialiseTriangleSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new TriangleWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new TriangleWaveSound());

	curWave = 2;
}

void SimonVstpluginAudioProcessor::initialiseSawSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new SawWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new SawWaveSound());

	curWave = 3;
}

AudioProcessor::BusesProperties SimonVstpluginAudioProcessor::getBusesProperties()
{
	return BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)
							.withOutput("Output", AudioChannelSet::stereo(), true);
}

//==============================================================================
const String SimonVstpluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimonVstpluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimonVstpluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double SimonVstpluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimonVstpluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimonVstpluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimonVstpluginAudioProcessor::setCurrentProgram (int index)
{
}

void SimonVstpluginAudioProcessor::setVoiceAmount(int amount)
{
	if (amount < 0 && amount < 17)
	{
		voiceAmount = amount;
	}
}

int SimonVstpluginAudioProcessor::getCurWave()
{
	return curWave;
}

const String SimonVstpluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimonVstpluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimonVstpluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	synth.setCurrentPlaybackSampleRate(sampleRate);
	keyboardState.reset();

	if (isUsingDoublePrecision())
	{
		delayBufferDouble.setSize(2, 12000);
		delayBufferFloat.setSize(1, 1);
	}
	else
	{
		delayBufferFloat.setSize(2, 12000);
		delayBufferDouble.setSize(1, 1);
	}

	reset();
}

void SimonVstpluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimonVstpluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

template <typename FloatType>
void SimonVstpluginAudioProcessor::process(AudioBuffer<FloatType>& buffer,
	MidiBuffer& midiMessages,
	AudioBuffer<FloatType>& delayBuffer)
{
	const int numSamples = buffer.getNumSamples();

	// Now pass any incoming midi messages to our keyboard state object, and let it
	// add messages to the buffer if the user is clicking on the on-screen keys
	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

	// and now get our synth to process these midi events and generate its output.
	synth.renderNextBlock(buffer, midiMessages, 0, numSamples);


	// Apply our delay effect to the new output..
	//applyDelay(buffer, delayBuffer);

	// In case we have more outputs than inputs, we'll clear any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		buffer.clear(i, 0, numSamples);

	//applyGain(buffer, delayBuffer); // apply our gain-change to the outgoing data..

									// Now ask the host for the current time so we can store it to be displayed later...
	//updateCurrentTimeInfoFromHost();
}

//==============================================================================
bool SimonVstpluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimonVstpluginAudioProcessor::createEditor()
{
    return new SimonVstpluginAudioProcessorEditor (*this);
}

//==============================================================================
void SimonVstpluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimonVstpluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimonVstpluginAudioProcessor();
}
