/*
  ==============================================================================

    EventBroadcasterEditor.cpp
    Created: 22 May 2015 3:34:30pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#include "DACoutFPGAEditor.h"
//#include "DACoutFPGACanvas.h"
#include "DACoutFPGA.h"

#include <stdio.h>
#include <string>



DACoutFPGAEditor::DACoutFPGAEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors = true)
	: GenericEditor(parentNode, useDefaultParameterEditors)

{
	desiredWidth = 200;

	processor = (DACoutFPGA*)getProcessor();

	inputSelect = new ComboBox("inputSelect");
	inputSelect->addListener(this);
	inputSelect->setBounds(70, 25, 100, 20);
	addAndMakeVisible(inputSelect);
	inputSelect->addItem("Generator", 1);
	inputSelect->addItem("Chain signal", 2);
	inputSelect->setSelectedId(1, sendNotification);

	pulsePeriod = new Label("pulsePeriod", "pulse period");
	pulsePeriod->setFont(Font("Default", 12, Font::plain));
	pulsePeriod->setEditable(true);
	pulsePeriod->setBounds(100, 50, 70, 20);
	pulsePeriod->addListener(this);
	pulsePeriod->setColour(Label::textColourId, Colours::white);
	pulsePeriod->setTooltip("Set the period of the DAC outputs in milliseconds");
	pulsePeriod->setText(String(1000), dontSendNotification);
	addAndMakeVisible(pulsePeriod);

	pulseDuration = new Label("pulseDuration", "Pulse Duration");
	pulseDuration->setFont(Font("Default", 12, Font::plain));
	pulseDuration->setEditable(true);
	pulseDuration->setBounds(100, 75, 70, 20);
	pulseDuration->addListener(this);
	pulseDuration->setColour(Label::textColourId, Colours::white);
	pulseDuration->setTooltip("Set the duration of the DAC pulse in milliseconds");
	pulseDuration->setText(String(500), dontSendNotification);
	addAndMakeVisible(pulseDuration);

	gain = new Label("gain", "gain of the output");
	gain->setFont(Font("Default", 12, Font::plain));
	gain->setEditable(true);
	gain->setBounds(100, 50, 70, 20);
	gain->addListener(this);
	gain->setColour(Label::textColourId, Colours::white);
	gain->setTooltip("Set the gain of the DAC output");
	gain->setText(String(16), dontSendNotification);
	addAndMakeVisible(gain);

	pulseAmplitude = new Label("pulseAmplitude", "pulse Amplitude");
	pulseAmplitude->setFont(Font("Default", 12, Font::plain));
	pulseAmplitude->setEditable(true);
	pulseAmplitude->setBounds(100, 100, 70, 20);
	pulseAmplitude->addListener(this);
	pulseAmplitude->setColour(Label::textColourId, Colours::white);
	pulseAmplitude->setTooltip("Set the amplitude of the DAC pulse");
	pulseAmplitude->setText(String(16), dontSendNotification);
	addAndMakeVisible(pulseAmplitude);


	inputLabel = new Label("inputLabel", "input label");
	inputLabel->setFont(Font("Default", 12, Font::plain));
	inputLabel->setEditable(false);
	inputLabel->setBounds(10, 25, 90, 20);
	inputLabel->setColour(Label::textColourId, Colours::white);
	inputLabel->setText("input: ", dontSendNotification);
	addAndMakeVisible(inputLabel);


	periodLabel = new Label("periodLabel", "Pulse Period");
	periodLabel->setFont(Font("Default", 12, Font::plain));
	periodLabel->setEditable(false);
	periodLabel->setBounds(10, 50, 90, 20);
	periodLabel->setColour(Label::textColourId, Colours::white);
	periodLabel->setText("period (ms): ", dontSendNotification);
	addAndMakeVisible(periodLabel);


	durationLabel = new Label("durationLabel", "Duration label");
	durationLabel->setFont(Font("Default", 12, Font::plain));
	durationLabel->setEditable(false);
	durationLabel->setBounds(10, 75, 90, 20);
	durationLabel->setColour(Label::textColourId, Colours::white);
	durationLabel->setText("duration (ms): ", dontSendNotification);
	addAndMakeVisible(durationLabel);

	gainLabel = new Label("gainLabel", "Gain label");
	gainLabel->setFont(Font("Default", 12, Font::plain));
	gainLabel->setEditable(false);
	gainLabel->setBounds(10, 50, 90, 20);
	gainLabel->setColour(Label::textColourId, Colours::white);
	gainLabel->setText("gain: ", dontSendNotification);
	addAndMakeVisible(gainLabel);

	amplitudeLabel = new Label("amplitudeLabel", "Amplitude label");
	amplitudeLabel->setFont(Font("Default", 12, Font::plain));
	amplitudeLabel->setEditable(false);
	amplitudeLabel->setBounds(10, 100, 90, 20);
	amplitudeLabel->setColour(Label::textColourId, Colours::white);
	amplitudeLabel->setText("amplitude (): ", dontSendNotification);
	addAndMakeVisible(amplitudeLabel);

}

DACoutFPGAEditor::~DACoutFPGAEditor()
{

}


void DACoutFPGAEditor::labelTextChanged(Label* label)
{
	uint16 oldPeriod = processor->getuint64Value("period");
	float oldGain = processor->getfloatValue("gain");
	float oldAmplitude = processor->getfloatValue("amplitude");
	if (label == pulsePeriod){
		if (label->getText().getIntValue() <= 5){
			CoreServices::sendStatusMessage("Cannot have period that fast");
			label->setText(String(oldPeriod), juce::NotificationType::dontSendNotification);
		}
		else{
			processor->setParameter(1, label->getText().getIntValue());
		}
	}
	else if (label == pulseDuration) {
		if (label->getText().getIntValue() <= 0) {
			CoreServices::sendStatusMessage("Cannot have pulse duration that small");
			label->setText(String(oldPeriod / 2), juce::NotificationType::dontSendNotification);
		}
		else {
			processor->setParameter(2, label->getText().getIntValue());
		}
	}
	else if (label == pulseAmplitude){
		if (abs(label->getText().getFloatValue()) > 65534/2) {
			CoreServices::sendStatusMessage("amplitude too big");
			label->setText(String(oldAmplitude), juce::NotificationType::dontSendNotification);
		}
		else{
			processor->setParameter(3, label->getText().getFloatValue());
		}
	}
	else if (label == gain) {
		if (label->getText().getFloatValue() <= 0) {
			CoreServices::sendStatusMessage("Cannot have gain negative or null");
			label->setText(String(oldGain), juce::NotificationType::dontSendNotification);
		}
		else {
			processor->setParameter(4, label->getText().getFloatValue());
		}
	}
}

void DACoutFPGAEditor::comboBoxChanged(ComboBox* comboBox)
{
	if (comboBox == inputSelect) {
		bool source = comboBox->getSelectedId() - 1;
		gain->setVisible(source);
		gainLabel->setVisible(source);
		pulsePeriod->setVisible(!source);
		pulseDuration->setVisible(!source);
		pulseAmplitude->setVisible(!source);
		periodLabel->setVisible(!source);
		durationLabel->setVisible(!source);
		amplitudeLabel->setVisible(!source);
		processor->setParameter(0, source);
	}
}

void  DACoutFPGAEditor::setLabel(char* str, int val)
{
	if (str == "pulseDuration") {
		pulseDuration->setText(String(val), juce::NotificationType::dontSendNotification);
	}
	else if (str == "periodPulses") {
		pulsePeriod->setText(String(val), juce::NotificationType::dontSendNotification);
	}
	
}


void  DACoutFPGAEditor::setLabel(char* str, float val)
{
	if (str == "amplitude") {
		pulseAmplitude->setText(std::to_string(val), juce::NotificationType::dontSendNotification);
	}
	else if (str == "gain") {
		gain->setText(std::to_string(val), juce::NotificationType::dontSendNotification);
	}

}

void  DACoutFPGAEditor::setCombo(char* str, int val)
{
	if (str == "source") {
		inputSelect->setSelectedId(val + 1);
	}

}