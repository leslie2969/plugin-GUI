/*
  ==============================================================================

    EventBroadcasterEditor.h
    Created: 22 May 2015 3:34:30pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#ifndef __TTLoutFPGAEDITOR_H_F0BD2DD9__
#define __TTLoutFPGAEDITOR_H_F0BD2DD9__

#include <VisualizerEditorHeaders.h>

class TTLoutFPGA;

/**

User interface for EvntTrigAvg

@see EvntTrigAvg, EvntTrigAvgCanvas
*/

class TTLoutFPGAEditor : public GenericEditor,
	public Label::Listener,
	public ComboBox::Listener
{

public:

	TTLoutFPGAEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
	virtual ~TTLoutFPGAEditor();
	void buttonEvent(Button* button);
	void labelTextChanged(Label* label);
	void sliderEvent(Slider* slider);
	void setPulsePeriod(int val);
	void setTrigger(int val);
	void setSource(int val);
	void setPulseDuration(int val);
	void comboBoxChanged(ComboBox* comboBox);
	void channelChanged(int chan, bool newState) override;
	void updateSettings();


private:
	struct EventSources
	{
		unsigned int eventIndex;
		unsigned int channel;
	};
	std::vector<EventSources> eventSourceArray;


	TTLoutFPGA* processor;
	ScopedPointer<ComboBox> triggerChannels, inputSelect;
	ScopedPointer<Label> pulsePeriod, pulseDuration, periodLabel, channelLabel, durationLabel, inputLabel;
	Font font;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLoutFPGAEditor);

};




#endif  // __EventsEvalEDITOR_H_F0BD2DD9__
