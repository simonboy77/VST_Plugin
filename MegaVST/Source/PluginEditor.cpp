#include "PluginProcessor.h"
#include "PluginEditor.h"

MegaVstAudioProcessorEditor::MegaVstAudioProcessorEditor (MegaVstAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p), midiKeyboard(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
  addAndMakeVisible(midiKeyboard);

  sineButton.setButtonText("Sine");
	sineButton.addListener(this);
	sineButton.setBounds(5, 5, 150, 60);
	addAndMakeVisible(sineButton);

	squareButton.setButtonText("Square");
	squareButton.addListener(this);
	squareButton.setBounds(160, 5, 150, 60);
	addAndMakeVisible(squareButton);

	triangleButton.setButtonText("Triangle");
	triangleButton.addListener(this);
	triangleButton.setBounds(5, 70, 150, 60);
	addAndMakeVisible(triangleButton);

	sawButton.setButtonText("Saw");
	sawButton.addListener(this);
	sawButton.setBounds(160, 70, 150, 60);
	addAndMakeVisible(sawButton);

  setSize (400, 300);
}

MegaVstAudioProcessorEditor::~MegaVstAudioProcessorEditor()
{
}

void MegaVstAudioProcessorEditor::paint (Graphics& g)
{
  g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

  g.setColour (Colours::white);
  g.setFont (15.0f);
  g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void MegaVstAudioProcessorEditor::resized()
{
  Rectangle<int> r(getLocalBounds().reduced(8));

  midiKeyboard.setBounds(r.removeFromBottom(70));

  r.removeFromTop(20);
  Rectangle<int> sliderArea(r.removeFromTop(60));

  processor.lastUIWidth = getWidth();
	processor.lastUIHeight = getHeight();
}

void MegaVstAudioProcessorEditor::setFittedText(String text)
{
	fittedText = text;
}

void MegaVstAudioProcessorEditor::buttonClicked(Button* button)
{
	int curWave = processor.getCurWave();

	if (button == &sineButton && curWave != 0)
	{
		setFittedText("init Sine");
		processor.initialiseSineSynth();
		setFittedText("Sine");
	}
	else if (button == &squareButton && curWave != 1)
	{
		setFittedText("init Square");
		processor.initialiseSquareSynth();
		setFittedText("Square");
	}
	else if (button == &triangleButton && curWave != 2)
	{
		setFittedText("init Triangle");
		processor.initialiseTriangleSynth();
		setFittedText("Triangle");
	}
	else if (button == &sawButton && curWave != 3)
	{
		setFittedText("init Saw");
		processor.initialiseSawSynth();
		setFittedText("Saw");
	}
}
