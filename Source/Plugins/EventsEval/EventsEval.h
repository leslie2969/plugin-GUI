/*
  ==============================================================================

    EventsEval.h
    Created: 07 July 2018 10:28:50am
    Author:  Melodie Borel

  ==============================================================================
*/

#ifndef EVENTSEVAL_H_INCLUDED
#define EVENTSEVAL_H_INCLUDED

#include <ProcessorHeaders.h>

#ifdef ZEROMQ
    #ifdef WIN32
        #include <zmq.h>
        #include <zmq_utils.h>
    #else
        #include <zmq.h>
    #endif
#endif

#include <memory>


class EventsEval : public GenericProcessor
{
public:
    EventsEval();

    AudioProcessorEditor* createEditor() override;

    int getListeningPort() const;
    void setListeningPort (int port, bool forceRestart = false);

    void process (AudioSampleBuffer& continuousBuffer) override;
    void handleEvent (const EventChannel* channelInfo, const MidiMessage& event, int samplePosition = 0) override;
	void handleSpike(const SpikeChannel* channelInfo, const MidiMessage& event, int samplePosition = 0) override;

    void saveCustomParametersToXml (XmlElement* parentElement) override;
    void loadCustomParametersFromXml() override;


private:
	void sendEvent(const MidiMessage& event, float eventSampleRate) const;
    static std::shared_ptr<void> getZMQContext();
    static void closeZMQSocket (void* socket);

    const std::shared_ptr<void> zmqContext;
    std::unique_ptr<void, decltype (&closeZMQSocket)> zmqSocket;
    int listeningPort;

};


#endif  // EVENTSEVAL_H_INCLUDED
