/*
  ==============================================================================

    EventBroadcasterEditor.h
    Created: 22 May 2015 3:34:30pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#ifndef EVENTSEVALEDITOR_H_INCLUDED
#define EVENTSEVALEDITOR_H_INCLUDED

#include <EditorHeaders.h>


/**

 User interface for the "EventBroadcaster" source node.

 @see EventBroadcaster

 */

class EventsEvalEditor : public GenericEditor, public Label::Listener
{
public:
	EventsEvalEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);

    void buttonEvent(Button* button) override;
    void labelTextChanged(juce::Label* label) override;

private:
    ScopedPointer<UtilityButton> restartConnection;
    ScopedPointer<Label> urlLabel;
    ScopedPointer<Label> portLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EventsEvalEditor);

};


#endif  // EVENTSEVALEDITOR_H_INCLUDED
