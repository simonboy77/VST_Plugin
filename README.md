# VST_Plugin
My very own VST Plugin


How to get the header files of the soundwaves working:

(You can find everything below and more in the src folder, check the PluginProcessor and PluginEditor classes)

-Put all the headers in the same folder as your PluginProcessor.cpp

-Include the headers like so:

#############################################################################################

#include "Sine.h"
#include "Square.h"
#include "Triangle.h"
#include "Saw.h"

#############################################################################################


-Add following functions to PluginProcessor Class:
(But replace SimonVstpluginAudioProcessor with your own AudioProcessor name)

#############################################################################################

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

#############################################################################################

curWave is used to check which wave is currently selected. This can be used to display that information on the UI. 
voiceAmount is used to change the amount of voices, I have a SetVoiceAmount(int) function for that.

-Put one of the above initialise funtions in the constructor if you want a wave to be selected on startup

-I change between waves using buttons on the UI
