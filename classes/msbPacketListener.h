#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

//TODO:this is windows territory!
#include "process.h"
#include "pilot.h"


class MsbPacketListener : public osc::OscPacketListener {
public:

    int readTo;
    int writeTo;
    string name;
    Pilot * myPilot;

    HANDLE * mutex;

protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m,const IpEndpointName& remoteEndpoint );

};
