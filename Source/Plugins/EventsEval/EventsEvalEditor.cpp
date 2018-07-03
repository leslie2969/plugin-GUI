/*
  ==============================================================================

    EventBroadcasterEditor.cpp
    Created: 22 May 2015 3:34:30pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#include "EventsEvalEditor.h"
#include "EventsEval.h"


EventsEvalEditor::EventsEvalEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors)
    : GenericEditor(parentNode, useDefaultParameterEditors)

{
    desiredWidth = 180;

    urlLabel = new Label("Port", "Port:");
    urlLabel->setBounds(20,80,140,25);
    addAndMakeVisible(urlLabel);
	EventsEval* p = (EventsEval*)getProcessor();

    restartConnection = new UtilityButton("Restart Connection",Font("Default", 15, Font::plain));
    restartConnection->setBounds(20,45,150,18);
    restartConnection->addListener(this);
    addAndMakeVisible(restartConnection);

    portLabel = new Label("Port", String(p->getListeningPort()));
    portLabel->setBounds(70,85,80,18);
    portLabel->setFont(Font("Default", 15, Font::plain));
    portLabel->setColour(Label::textColourId, Colours::white);
    portLabel->setColour(Label::backgroundColourId, Colours::grey);
    portLabel->setEditable(true);
    portLabel->addListener(this);
    addAndMakeVisible(portLabel);

    setEnabledState(false);
}


void EventsEvalEditor::buttonEvent(Button* button)
{
    if (button == restartConnection)
    {
		EventsEval* p = (EventsEval*)getProcessor();
        p->setListeningPort(p->getListeningPort(), true);
    }
}


void EventsEvalEditor::labelTextChanged(juce::Label* label)
{
    if (label == portLabel)
    {
        Value val = label->getTextValue();

		EventsEval* p = (EventsEval*)getProcessor();
        p->setListeningPort(val.getValue());
    }
}
