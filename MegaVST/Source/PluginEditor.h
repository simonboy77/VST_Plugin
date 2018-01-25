#ifndef PLUGINEDITOR_H
#define PLUGINEDITOR_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class MegaVstAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener
{
public:
  MegaVstAudioProcessorEditor (MegaVstAudioProcessor&);
  ~MegaVstAudioProcessorEditor();

  void paint (Graphics&) override;
  void resized() override;

  void setFittedText(String text);
  void buttonClicked(Button* button);

private:
  MegaVstAudioProcessor& processor;

  MidiKeyboardComponent midiKeyboard;

  TextButton sineButton;
  TextButton squareButton;
  TextButton triangleButton;
  TextButton sawButton;

  String fittedText;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MegaVstAudioProcessorEditor)
};

#endif
