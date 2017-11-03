/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class SimonVstpluginAudioProcessorEditor  : public AudioProcessorEditor, private Timer, public Button::Listener
{
public:
    SimonVstpluginAudioProcessorEditor (SimonVstpluginAudioProcessor&);
    ~SimonVstpluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;
	void setFittedText(String text);

	void buttonClicked(Button* button);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	MidiKeyboardComponent midiKeyboard;
	Label label;
	TextButton sineButton;
	TextButton squareButton;
	TextButton triangleButton;
	TextButton sawButton;
    SimonVstpluginAudioProcessor& processor;

	String fittedText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimonVstpluginAudioProcessorEditor)
};
