/*
  ==============================================================================

    EventBroadcaster.cpp
    Created: 22 May 2015 3:31:50pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#include <stdio.h>
#include "TTLoutFPGA.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>


class TTLoutFPGA;

TTLoutFPGA::TTLoutFPGA()
	: GenericProcessor("TTL out FPGA")
{
	setProcessorType(PROCESSOR_TYPE_SINK);
	pulseDuration = 500; // 500 ms
	pulsePeriod = 1000; // 1000 milliseconds
	ThetriggerChannel = 1;
	TheInputSource = 1;
	updateSettings();
}

TTLoutFPGA::~TTLoutFPGA()
{
}

void TTLoutFPGA::process(AudioSampleBuffer& buffer)
{
	checkForEvents(); // see if we got any TTL events
}

void TTLoutFPGA::handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int sampleNum)
{
	if (TheInputSource == 1) {
		//CoreServices::setTTLoutStatus(1, ThetriggerChannel);
		BinaryEventPtr ttl = BinaryEvent::deserializeFromMessage(event, eventInfo);
		//SpikeEventPtr newSpike = SpikeEvent::deserializeFromMessage(event, spikeInfo);
		if (!ttl)
			return;
		else {
				CoreServices::setTTLoutStatus(1, ThetriggerChannel);
		}
	}
}


void TTLoutFPGA::timerCallback()
{
	if (TheInputSource == 0) {
		CoreServices::setTTLoutStatus(1, ThetriggerChannel);
	}
}

void TTLoutFPGA::setParameter(int parameterIndex, float newValue)
{
	bool changed = false;
	if (parameterIndex == 0 && ThetriggerChannel != newValue) {
		ThetriggerChannel = newValue;
		changed = true;
	}
	else if (parameterIndex == 1 && pulsePeriod != newValue){
		pulsePeriod = newValue;
		stopTimer();
		startTimer(pulsePeriod);
		changed = true;
	}
	else if (parameterIndex == 2 && pulseDuration != newValue){
		pulseDuration = newValue;
		changed = true;
	}
	else if (parameterIndex == 3 && TheInputSource != newValue) {
		TheInputSource = newValue;
		if (TheInputSource == 0) {
			startTimer(pulsePeriod);
		}
		else if (TheInputSource == 1) {
			stopTimer();
		}
		changed = true;
	}

	// If anything was changed, delete all data and start over
	if (changed){
		updateSettings();
	}
}

uint32 TTLoutFPGA::getChannelSourceID(const EventChannel* event) const
{
	int metaDataIndex = event->findMetaData(MetaDataDescriptor::UINT16, 3, "source.channel.identifier.full");
	if (metaDataIndex < 0)
	{
		return getProcessorFullId(event->getSourceNodeID(), event->getSubProcessorIdx());
	}
	uint16 values[3];
	event->getMetaDataValue(metaDataIndex)->getValue(static_cast<uint16*>(values));
	return getProcessorFullId(values[1], values[2]);
}


void TTLoutFPGA::updateSettings()
{
	channelForEventSource.clear();
	eventSourceNodes.clear();
	ttlState.clear();

	for (int i = 0; i < eventChannelArray.size(); ++i)
	{
		uint32 sourceID = getChannelSourceID(eventChannelArray[i]);
		if (!eventSourceNodes.contains(sourceID))
		{
			eventSourceNodes.add(sourceID);

		}
	}

	
	numEventChannels = eventSourceNodes.size();

	std::cout << "Found " << numEventChannels << " event channels." << std::endl;

	for (int i = 0; i < eventSourceNodes.size(); ++i)
	{
		std::cout << "Adding channel " << getNumInputs() + i << " for event source node " << eventSourceNodes[i] << std::endl;

		channelForEventSource[eventSourceNodes[i]] = getNumInputs() + i;
		ttlState[eventSourceNodes[i]] = 0;
	}

	displayBufferIndex.clear();
	displayBufferIndex.insertMultiple(0, 0, getNumInputs() + numEventChannels);

	// update the editor's subprocessor selection display
	//TTLoutFPGAEditor * ed = (TTLoutFPGAEditor*)getEditor();
	//ed->updateSubprocessorSelectorOptions();

	CoreServices::setTTLoutLength(pulseDuration, ThetriggerChannel);

}

bool TTLoutFPGA::enable()
{
	startTimer(pulsePeriod);
	return true;
}

bool TTLoutFPGA::disable()
{
	stopTimer();
	return true;
}


//AudioProcessorEditor* TTLoutFPGA::createEditor()
AudioProcessorEditor* TTLoutFPGA::createEditor()
{
	editor = new TTLoutFPGAEditor(this, true);
	return editor;
}

float TTLoutFPGA::getSampleRate()
{
	return juce::AudioProcessor::getSampleRate();
}

uint64 TTLoutFPGA::getpulsePeriod()
{
	return pulsePeriod;
}

uint64 TTLoutFPGA::getpulseDuration()
{
	return pulseDuration;
}



void TTLoutFPGA::saveCustomParametersToXml(XmlElement* parentElement)
{
	XmlElement* mainNode = parentElement->createNewChildElement("TTLOUTFPGA");
	mainNode->setAttribute("trigger", ThetriggerChannel);
	mainNode->setAttribute("sourcetrig", TheInputSource);
	mainNode->setAttribute("period", int(pulsePeriod));
	mainNode->setAttribute("duration", int(pulseDuration));
}

void TTLoutFPGA::loadCustomParametersFromXml()
{
	if (parametersAsXml)
	{
		TTLoutFPGAEditor* ed = (TTLoutFPGAEditor*)getEditor();

		forEachXmlChildElement(*parametersAsXml, mainNode)
		{
			if (mainNode->hasTagName("TTLOUTFPGA"))
			{
				ThetriggerChannel = mainNode->getIntAttribute("trigger");
				std::cout << "set trigger channel to: " << ThetriggerChannel << "\n";
				ed->setTrigger(mainNode->getIntAttribute("trigger"));

				TheInputSource = mainNode->getIntAttribute("sourcetrig");
				std::cout << "set trigger source to: " << TheInputSource << "\n";
				ed->setSource(mainNode->getIntAttribute("sourcetrig"));

				pulsePeriod = mainNode->getIntAttribute("period");
				std::cout << "set pulses period to: " << pulsePeriod << "\n";
				ed->setPulsePeriod(mainNode->getIntAttribute("period"));

				pulseDuration = uint64(mainNode->getIntAttribute("duration"));
				std::cout << "set pulse duration to: " << pulseDuration << "\n";
				ed->setPulseDuration(mainNode->getIntAttribute("duration"));
			}
		}
	}
}

