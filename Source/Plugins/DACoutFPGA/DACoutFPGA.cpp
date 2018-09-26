/*
  ==============================================================================

    EventBroadcaster.cpp
    Created: 22 May 2015 3:31:50pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#include <stdio.h>
#include "DACoutFPGA.h"
//#include "DACoutFPGACanvas.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>


class DACoutFPGA;

DACoutFPGA::DACoutFPGA()
	: GenericProcessor("DAC out FPGA")

{
	setProcessorType(PROCESSOR_TYPE_SINK);
	amplitude = 16; //percentage of 5V
	pulseDuration=500;
	periodPulses=1000;
	gain = 400; // pA/V in multiclamp
	ison = false;
	ThetriggerChannel = 7;
	CoreServices::setTTLoutLength(1, ThetriggerChannel);
	updateSettings();


}

DACoutFPGA::~DACoutFPGA()
{
}

void DACoutFPGA::setParameter(int parameterIndex, float newValue)
{
	bool changed = false;
	if (parameterIndex == 0 && source != newValue){
		source = newValue;
		stopTimer();
		if(source==0){
			startTimer(periodPulses);
		}
		changed = true;
	}
	else if (parameterIndex == 1 && periodPulses != newValue){
		periodPulses = newValue;
		stopTimer();
		if (source == 0) {
			startTimer(periodPulses);
		}
		changed = true;
	}
	else if (parameterIndex == 2 && pulseDuration != newValue){
		pulseDuration = newValue;

		changed = true;
	}
	else if (parameterIndex == 3 && amplitude != newValue) {
		amplitude = newValue;
		changed = true;
	}
	else if (parameterIndex == 4 && gain != newValue){
		gain = newValue;
		changed = true;
	}

	// If anything was changed, delete all data and start over
	if (changed){
		updateSettings();
	}
}

bool DACoutFPGA::enable()
{
	if (source == 0) {
		startTimer(periodPulses);
	}
	return true;
}

bool DACoutFPGA::disable()
{
	stopTimer();
	int dacToSend = 65534 / 2;
	CoreServices::setManualDACStatus(dacToSend);
	return true;
}

void DACoutFPGA::updateSettings()
{

}

void DACoutFPGA::timerCallback()
{
	int status = CoreServices::getManualDACStatus();
	if (status == 65534/2){
		stopTimer();
		CoreServices::setManualDACStatus(int(65534 / 2 * (1 + amplitude /100)));
		startTimer(pulseDuration);
	}
	else {
		stopTimer();
		CoreServices::setManualDACStatus(int(65534/2));
		startTimer(periodPulses-pulseDuration);
	}
		
}

void DACoutFPGA::process(AudioSampleBuffer& buffer)
{
	// 1. place any new samples into the displayBuffer
	//std::cout << "Display node sample count: " << nSamples << std::endl; ///buffer.getNumSamples() << std::endl;
	

	ScopedLock displayLock(mut);
	if (source == 1) {
		int last = buffer.getNumSamples();

		std::cout << "numpnts : " << last << std::endl;

		float datap = buffer.getSample(0, 1023)/16;
		if (datap >= 0.5 && !ison) {
			CoreServices::setTTLoutStatus(1, ThetriggerChannel);
			ison = true;
		}
		else if (datap < 0.5 && ison) {
			CoreServices::setTTLoutStatus(0, ThetriggerChannel);
			ison = false;
		}
		else {
			
		}
		//int dacToSend = 65534 / 2 * (1 + datap /gain/5) ;
		int dacToSend = int(65534 / 2 * (1 + datap * gain / 100));
		//std::cout << "Display node samplecount: " << datap << std::endl;
		if (dacToSend >= 0 && dacToSend <= 65535) {
			CoreServices::setManualDACStatus(dacToSend);
		}
		else {
			std::cout << "DAC output out of range" << std::endl;
		}
	}

}


//AudioProcessorEditor* EventsEval::createEditor()
AudioProcessorEditor* DACoutFPGA::createEditor()
{
	editor = new DACoutFPGAEditor(this, true);
	return editor;
}

float DACoutFPGA::getSampleRate()
{
	return juce::AudioProcessor::getSampleRate();
}


float DACoutFPGA::getfloatValue(char* label) {
	if (label == "gain") {
		return gain;
	}
	else if (label == "amplitude") {
		return amplitude;
	}
}

uint64 DACoutFPGA::getuint64Value(char* label){
	if (label == "period") {
		return periodPulses;
	}
	else if (label == "duration") {
		return pulseDuration;
	}
}

int DACoutFPGA::getintValue(char* label) {
	if (label == "source") {
		return source;
	}
}

void DACoutFPGA::saveCustomParametersToXml(XmlElement* parentElement)
{
	XmlElement* mainNode = parentElement->createNewChildElement("DACOUTFPGA");
	mainNode->setAttribute("source", int(source));
	mainNode->setAttribute("periodPulses", int(periodPulses));
	mainNode->setAttribute("pulseDuration", int(pulseDuration));
	mainNode->setAttribute("amplitude", int(amplitude));
	mainNode->setAttribute("gain", int(gain*1000));

}

void DACoutFPGA::loadCustomParametersFromXml()
{
	if (parametersAsXml)
	{
		DACoutFPGAEditor* ed = (DACoutFPGAEditor*)getEditor();

		forEachXmlChildElement(*parametersAsXml, mainNode)
		{
			if (mainNode->hasTagName("DACOUTFPGA"))
			{
				source = bool(mainNode->getIntAttribute("source"));
				std::cout << "set source to: " << source << "\n";
				ed->setCombo("source", source);

				periodPulses = uint64(mainNode->getIntAttribute("periodPulses"));
				std::cout << "set pulse period to: " << periodPulses << "\n";
				ed->setLabel("periodPulses",mainNode->getIntAttribute("periodPulses"));

				pulseDuration = uint64(mainNode->getIntAttribute("pulseDuration"));
				std::cout << "set pulse duration to: " << pulseDuration << "\n";
				ed->setLabel("pulseDuration", mainNode->getIntAttribute("pulseDuration"));

				amplitude = mainNode->getIntAttribute("amplitude");
				std::cout << "set pulse amplitude to: " << amplitude << "\n";
				ed->setLabel("amplitude", amplitude);

				gain = mainNode->getIntAttribute("gain");
				gain = float(gain / 1000);
				std::cout << "set gain to: " << gain << "\n";
				ed->setLabel("gain", gain);
			}
		}
	}
}

