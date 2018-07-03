/*
  ==============================================================================

    EventBroadcaster.cpp
    Created: 22 May 2015 3:31:50pm
    Author:  Christopher Stawarz

  ==============================================================================
*/

#include "EventsEval.h"
#include "EventsEvalEditor.h"

std::shared_ptr<void> EventsEval::getZMQContext() {
    // Note: C++11 guarantees that initialization of static local variables occurs exactly once, even
    // if multiple threads attempt to initialize the same static local variable concurrently.
#ifdef ZEROMQ
    static const std::shared_ptr<void> ctx(zmq_ctx_new(), zmq_ctx_destroy);
#else
    static const std::shared_ptr<void> ctx;
#endif
    return ctx;
}


void EventsEval::closeZMQSocket(void* socket)
{
#ifdef ZEROMQ
    zmq_close(socket);
#endif
}


EventsEval::EventsEval()
    : GenericProcessor  ("Events Eval")
    , zmqContext        (getZMQContext())
    , zmqSocket         (nullptr, &closeZMQSocket)
    , listeningPort     (0)
{
    setProcessorType (PROCESSOR_TYPE_SINK);

    setListeningPort(5557);
}


AudioProcessorEditor* EventsEval::createEditor()
{
    editor = new EventsEvalEditor(this, true);
    return editor;
}


int EventsEval::getListeningPort() const
{
    return listeningPort;
}


void EventsEval::setListeningPort(int port, bool forceRestart)
{
    if ((listeningPort != port) || forceRestart)
    {
#ifdef ZEROMQ
        zmqSocket.reset(zmq_socket(zmqContext.get(), ZMQ_PUB));
        if (!zmqSocket)
        {
            std::cout << "Failed to create socket: " << zmq_strerror(zmq_errno()) << std::endl;
            return;
        }

        String url = String("tcp://*:") + String(port);
        if (0 != zmq_bind(zmqSocket.get(), url.toRawUTF8()))
        {
            std::cout << "Failed to open socket: " << zmq_strerror(zmq_errno()) << std::endl;
            return;
        }
#endif

        listeningPort = port;
    }
}


void EventsEval::process(AudioSampleBuffer& continuousBuffer)
{
    checkForEvents(true);
}


//IMPORTANT: The structure of the event buffers has changed drastically, so we need to find a better way of doing this
void EventsEval::sendEvent(const MidiMessage& event, float eventSampleRate) const
{
	double timestampSeconds = double(Event::getTimestamp(event)) / eventSampleRate;
	uint16 type = Event::getBaseType(event);

#ifdef ZEROMQ
	if (-1 == zmq_send(zmqSocket.get(), &type, sizeof(type), ZMQ_SNDMORE) ||
		-1 == zmq_send(zmqSocket.get(), &timestampSeconds, sizeof(timestampSeconds), ZMQ_SNDMORE) ||
		-1 == zmq_send(zmqSocket.get(), event.getRawData(), event.getRawDataSize(), 0))
	{
		std::cout << "Failed to send message: " << zmq_strerror(zmq_errno()) << std::endl;
	}
#endif
}

void EventsEval::handleEvent(const EventChannel* channelInfo, const MidiMessage& event, int samplePosition)
{
	sendEvent(event, channelInfo->getSampleRate());
}

void EventsEval::handleSpike(const SpikeChannel* channelInfo, const MidiMessage& event, int samplePosition)
{
	sendEvent(event, channelInfo->getSampleRate());
}

void EventsEval::saveCustomParametersToXml(XmlElement* parentElement)
{
    XmlElement* mainNode = parentElement->createNewChildElement("EVENTSEVAL");
    mainNode->setAttribute("port", listeningPort);
}


void EventsEval::loadCustomParametersFromXml()
{
    if (parametersAsXml)
    {
        forEachXmlChildElement(*parametersAsXml, mainNode)
        {
            if (mainNode->hasTagName("EVENTSEVAL"))
            {
                setListeningPort(mainNode->getIntAttribute("port"));
            }
        }
    }
}
