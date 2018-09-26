/*
  ==============================================================================

    EventBroadcasterEditor.cpp
    Created: 22 May 2015 3:34:30pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#include "TTLoutFPGAEditor.h"
#include "TTLoutFPGA.h"

#include <stdio.h>



TTLoutFPGAEditor::TTLoutFPGAEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors = true)
	: GenericEditor(parentNode, useDefaultParameterEditors)

{
	desiredWidth = 200;

	processor = (TTLoutFPGA*)getProcessor();

	triggerChannels = new ComboBox("triggerChannels");
	triggerChannels->addListener(this);
	triggerChannels->setBounds(110, 25, 60, 20);
	addAndMakeVisible(triggerChannels);
	triggerChannels->addItem("None", 1);
	triggerChannels->setSelectedId(1, sendNotification);

	inputSelect = new ComboBox("inputSelect");
	inputSelect->addListener(this);
	inputSelect->setBounds(70, 50, 100, 20);
	addAndMakeVisible(inputSelect);
	inputSelect->addItem("Generator", 1);
	inputSelect->addItem("Chain events", 2);
	inputSelect->setSelectedId(1, sendNotification);

	pulsePeriod = new Label("pulsePeriod", "pulse period");
	pulsePeriod->setFont(Font("Default", 12, Font::plain));
	pulsePeriod->setEditable(true);
	pulsePeriod->setBounds(130, 75, 80, 20);
	pulsePeriod->addListener(this);
	pulsePeriod->setColour(Label::textColourId, Colours::white);
	pulsePeriod->setTooltip("Set the period of the TTL outputs in milliseconds");
	pulsePeriod->setText(String(1000), dontSendNotification);
	addAndMakeVisible(pulsePeriod);

	pulseDuration = new Label("pulseDuration", "Pulse Duration");
	pulseDuration->setFont(Font("Default", 12, Font::plain));
	pulseDuration->setEditable(true);
	pulseDuration->setBounds(130, 100, 80, 20);
	pulseDuration->addListener(this);
	pulseDuration->setColour(Label::textColourId, Colours::white);
	pulseDuration->setTooltip("Set the duration of the TTL pulse in milliseconds");
	pulseDuration->setText(String(500), dontSendNotification);
	addAndMakeVisible(pulseDuration);


	channelLabel = new Label("channelLabel", "channel label");
	channelLabel->setFont(Font("Default", 12, Font::plain));
	channelLabel->setEditable(false);
	channelLabel->setBounds(10, 25, 90, 20);
	channelLabel->setColour(Label::textColourId, Colours::white);
	channelLabel->setText("Channel out: ", dontSendNotification);
	addAndMakeVisible(channelLabel);

	inputLabel = new Label("inputLabel", "input label");
	inputLabel->setFont(Font("Default", 12, Font::plain));
	inputLabel->setEditable(false);
	inputLabel->setBounds(10, 50, 90, 20);
	inputLabel->setColour(Label::textColourId, Colours::white);
	inputLabel->setText("input: ", dontSendNotification);
	addAndMakeVisible(inputLabel);


	periodLabel = new Label("periodLabel", "Pulse Period");
	periodLabel->setFont(Font("Default", 12, Font::plain));
	periodLabel->setEditable(false);
	periodLabel->setBounds(10, 75, 110, 20);
	periodLabel->setColour(Label::textColourId, Colours::white);
	periodLabel->setText("TTL period (ms): ", dontSendNotification);
	addAndMakeVisible(periodLabel);


	durationLabel = new Label("durationLabel", "Duration label");
	durationLabel->setFont(Font("Default", 12, Font::plain));
	durationLabel->setEditable(false);
	durationLabel->setBounds(10, 100, 110, 20);
	durationLabel->setColour(Label::textColourId, Colours::white);
	durationLabel->setText("TTL duration (ms): ", dontSendNotification);
	addAndMakeVisible(durationLabel);

}



TTLoutFPGAEditor::~TTLoutFPGAEditor()
{

}

void TTLoutFPGAEditor::sliderEvent(Slider* slider)
{

}


void TTLoutFPGAEditor::buttonEvent(Button* button)
{
}

void TTLoutFPGAEditor::labelTextChanged(Label* label)
{
	uint64 wS = processor->getpulseDuration();
	uint64 wms = wS / (processor->getSampleRate() / 1000);
	uint64 fG = processor->getpulsePeriod();
	if (label == pulsePeriod){
		if (label->getText().getIntValue() == 0){
			CoreServices::sendStatusMessage("Cannot have update that fast");
			label->setText(String(fG), juce::NotificationType::dontSendNotification);
		}
		else if ((label->getText().getIntValue()) <= 30000)
			processor->setParameter(1, label->getText().getIntValue());
		else{
			CoreServices::sendStatusMessage("Doessn't make sens to have such a slow uppdate");
			label->setText(String(fG), juce::NotificationType::dontSendNotification);
		}

	}
	else if (label == pulseDuration){
		if (label->getText().getIntValue() == 0){
			CoreServices::sendStatusMessage("Cannot have window size of 0");
		}
		else {
			processor->setParameter(2, label->getText().getIntValue());
		}
	}
}


void TTLoutFPGAEditor::updateSettings()
{
	EventSources s;
	String name;
	int oldId = triggerChannels->getSelectedId();
	triggerChannels->clear();
	triggerChannels->addItem("None", 1);
	int nextItem = 2;
	int nTTLsOut = 8;
	for (int i = 0; i < nTTLsOut; i++)
	{
		name = "TTL " + String(i + 1);
		triggerChannels->addItem(name, nextItem++);

	}
	if (oldId > triggerChannels->getNumItems())
	{
		oldId = 1;
	}
	triggerChannels->setSelectedId(oldId, sendNotification);
}

//newValue*(getSampleRate()/1000);

void TTLoutFPGAEditor::comboBoxChanged(ComboBox* comboBox)
{
	if (comboBox == triggerChannels) {
		if (comboBox->getSelectedId() > 1)
		{
			int index = comboBox->getSelectedId() - 2;
			processor->setParameter(0, index);
		}
		else
			getProcessor()->setParameter(0, -1);
	}
	else if (comboBox == inputSelect) {
		int index = comboBox->getSelectedId() - 1;
		processor->setParameter(3, index);
	}
}

void TTLoutFPGAEditor::channelChanged(int chan, bool newState) {

}
//newValue*(getSampleRate()/1000);


void  TTLoutFPGAEditor::setPulsePeriod(int val)
{
	pulsePeriod->setText(String(val), juce::NotificationType::dontSendNotification);
}
void  TTLoutFPGAEditor::setPulseDuration(int val)
{
	pulseDuration->setText(String(val), juce::NotificationType::dontSendNotification);
}
void  TTLoutFPGAEditor::setTrigger(int val)
{
	triggerChannels->setSelectedId(val + 2);
}
void  TTLoutFPGAEditor::setSource(int val)
{
	inputSelect->setSelectedId(val+1);
}