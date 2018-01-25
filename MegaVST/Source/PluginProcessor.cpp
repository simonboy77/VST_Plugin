#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Sine.h"
#include "Square.h"
#include "Triangle.h"
#include "Saw.h"

MegaVstAudioProcessor::MegaVstAudioProcessor()
     : AudioProcessor (getBusesProperties())
{
	voiceAmount = 8;

	initialiseSineSynth();
}

MegaVstAudioProcessor::~MegaVstAudioProcessor()
{
}

void MegaVstAudioProcessor::ResetSynth()
{
	//synth.allNotesOff();
	synth.clearSounds();
	synth.clearVoices();
}

void MegaVstAudioProcessor::initialiseSineSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new SineWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new SineWaveSound());

	curWave = 0;
}

void MegaVstAudioProcessor::initialiseSquareSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new SquareWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new SquareWaveSound());

	curWave = 1;
}

void MegaVstAudioProcessor::initialiseTriangleSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new TriangleWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new TriangleWaveSound());

	curWave = 2;
}

void MegaVstAudioProcessor::initialiseSawSynth()
{
	ResetSynth();

	// Add some voices...
	for (int i = voiceAmount; --i >= 0;)
		synth.addVoice(new SawWaveVoice());

	// ..and give the synth a sound to play
	synth.addSound(new SawWaveSound());

	curWave = 3;
}

AudioProcessor::BusesProperties MegaVstAudioProcessor::getBusesProperties()
{
	return BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)
							.withOutput("Output", AudioChannelSet::stereo(), true);
}

//==============================================================================
const String MegaVstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MegaVstAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MegaVstAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double MegaVstAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MegaVstAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MegaVstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MegaVstAudioProcessor::setCurrentProgram (int index)
{
}

void MegaVstAudioProcessor::setVoiceAmount(int amount)
{
	if (amount < 0 && amount < 17)
	{
		voiceAmount = amount;
	}
}

int MegaVstAudioProcessor::getCurWave()
{
	return curWave;
}

const String MegaVstAudioProcessor::getProgramName (int index)
{
    return {};
}

void MegaVstAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MegaVstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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

void MegaVstAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MegaVstAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
void MegaVstAudioProcessor::process(AudioBuffer<FloatType>& buffer,
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
bool MegaVstAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MegaVstAudioProcessor::createEditor()
{
    return new MegaVstAudioProcessorEditor (*this);
}

//==============================================================================
void MegaVstAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MegaVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MegaVstAudioProcessor();
}
