/*
  ==============================================================================

    EventsEval.h
    Created: 07 July 2018 10:28:50am
    Author:  Melodie Borel

  ==============================================================================
*/

#ifndef __TTLoutFPGA_H_3F920F95__
#define __TTLoutFPGA_H_3F920F95__

#include <ProcessorHeaders.h>
#include "TTLoutFPGAEditor.h"
#include <vector>
#include <map>


class TTLoutFPGAEditor;

/**
Aligns spike times with TTL input.

@see EvntTrigAvgCanvas, EvntTrigAvgEditor

*/

class TTLoutFPGA : public GenericProcessor, private Timer
{
public:

	/** constructor */
	TTLoutFPGA();

	/** destructor */
	~TTLoutFPGA();

	void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int sampleNum) override;
	void process(AudioSampleBuffer& buffer) override;


	/** Used to alter parameters of data acquisition. */
	void setParameter(int parameterIndex, float newValue) override;

	/** Called whenever the signal chain is altered. */
	void updateSettings() override;

	/** Called prior to start of acquisition. */
	bool enable() override;

	/** Called after acquisition is finished. */
	bool disable() override;

	/** Creates the EventsEvalEditor. */
	AudioProcessorEditor* createEditor() override;
	void timerCallback() override;

	//int checkForEvents(bool checkForSpikes) override;

	float getSampleRate();
	uint64 getpulseDuration();
	uint64 getpulsePeriod();

	CriticalSection* getMutex() { return &mut; }

	void saveCustomParametersToXml(XmlElement* parentElement) override;
	void loadCustomParametersFromXml() override;
private:
	CriticalSection mut;

	void initializeEventChannels();
	void finalizeEventChannels();

	uint64 pulseDuration;
	uint64 pulsePeriod;

	ScopedPointer<AudioSampleBuffer> displayBuffer;
	Array<int> displayBufferIndex;
	Array<uint32> eventSourceNodes;
	std::map<uint32, int> channelForEventSource;

	float* arrayOfOnes;

	std::map<uint32, uint64> ttlState;
	int numEventChannels;
	int ThetriggerChannel;
	int TheInputSource;
	int lastSpikeID;
	uint32 getChannelSourceID(const EventChannel* event) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLoutFPGA);

};



#endif  // __TTLoutFPGA_H_3F920F95__
