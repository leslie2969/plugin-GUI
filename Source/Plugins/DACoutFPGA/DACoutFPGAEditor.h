/*
  ==============================================================================

    EventBroadcasterEditor.h
    Created: 22 May 2015 3:34:30pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#ifndef __DACoutFPGAEDITOR_H_F0BD2DD9__
#define __DACoutFPGAEDITOR_H_F0BD2DD9__

#include <VisualizerEditorHeaders.h>


//class DACoutFPGACanvas;
class DACoutFPGA;

/**

User interface for EvntTrigAvg

@see EvntTrigAvg, EvntTrigAvgCanvas
*/

class DACoutFPGAEditor : public GenericEditor,
	public Label::Listener,
	public ComboBox::Listener
{

public:

	DACoutFPGAEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
	virtual ~DACoutFPGAEditor();
	void labelTextChanged(Label* label);
	void comboBoxChanged(ComboBox* comboBox);
	void setLabel(char* str,int val);
	void setLabel(char* str, float val);
	void setCombo(char* str, int val);

private:

	DACoutFPGA* processor;
	ScopedPointer<Label> pulsePeriod, pulseDuration, gain, pulseAmplitude, inputLabel, periodLabel, durationLabel, gainLabel, amplitudeLabel;
	ScopedPointer<ComboBox> inputSelect;
	Font font;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DACoutFPGAEditor);

};




#endif  // __EventsEvalEDITOR_H_F0BD2DD9__
