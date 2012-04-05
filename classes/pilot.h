#ifndef H_PILOT
#define H_PILOT

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"


#ifdef TARGET_WIN32
	#include "process.h"
#endif

#include "actor.h"
#include "msbObject.h"


class UdpInput;
class Input;

//***************************************************************************************
//  Pilot class
//
//  interprets OSC (or other formats) messages into moviesandbox strings
//  that can then be converted to Actor Properties using the memberFromString() function
//
//  * is threaded
//
//  * creates inputconnectButtons through UdpInput
//
//***************************************************************************************

class Pilot : public osc::OscPacketListener, public MsbObject{
public:

    Input*          input;

    UdpInput*       parent;             //link back to the UdpInput Button

#ifdef TARGET_WIN32
    HANDLE *        mutex;              //the thread MUTEX
#endif

#ifdef TARGET_MACOSX
	pthread_mutex_t * mutex;
#endif

#ifdef TARGET_LINUX
	pthread_mutex_t * mutex;
#endif

    std::string     name;               //simple identifier

    Pilot();

protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m,const IpEndpointName& remoteEndpoint );

    virtual void setPropertyForSelected(const osc::ReceivedMessage& m, char* adressPart, string partAsString);
    virtual void switchCameraToSelected(const osc::ReceivedMessage& m, char* adressPart, string partAsString);
    virtual void selectPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString);
    virtual void playPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString);
    virtual void triggerPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString);
    virtual void brushPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString);
    virtual void pilotPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString);

    virtual void update(float deltaTime);

};
#endif
