/*
  ==============================================================================

    EventsEval.h
    Created: 07 July 2018 10:28:50am
    Author:  Melodie Borel

  ==============================================================================
*/

#ifndef __DACoutFPGA_H_3F920F95__
#define __DACoutFPGA_H_3F920F95__

#include <ProcessorHeaders.h>
#include "DACoutFPGAEditor.h"
#include <vector>
#include <map>


class DACoutFPGAEditor;

/**
Aligns spike times with TTL input.

@see EvntTrigAvgCanvas, EvntTrigAvgEditor

*/

class DACoutFPGA : public GenericProcessor, private Timer
{
public:

	/** constructor */
	DACoutFPGA();

	/** destructor */
	~DACoutFPGA();


	// PROCESSOR METHODS //
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
	//AudioProcessorEditor* createEditor() override;
	AudioProcessorEditor* createEditor() override;
	void timerCallback() override;

	float getSampleRate();

	float getfloatValue(char* label);
	int getintValue(char* label);
	uint64 getuint64Value(char* label);

	bool ison;
	int ThetriggerChannel;

	CriticalSection* getMutex() { return &mut; }

	void saveCustomParametersToXml(XmlElement* parentElement) override;
	void loadCustomParametersFromXml() override;
private:
	CriticalSection mut;

	std::atomic<int> triggerEvent;

	float amplitude;
	uint64 pulseDuration;
	uint64 periodPulses;
	float gain;
	int source;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DACoutFPGA);

};



#endif  // __DACoutFPGA_H_3F920F95__
