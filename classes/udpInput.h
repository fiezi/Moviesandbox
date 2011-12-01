#ifndef H_udpInput
#define H_udpInput


#include "renderer.h"
#include "inputConnectButton.h"
#include "pilot.h"

//************************************************************
//  UDPInput is a Button that represents a stream of OSC messages coming in through one UDP port
//
//  port = 31840+channel
//
//  * holds a list of inputConnectButtons
//
//  * creates a seperate pilot thread as interpreter for the incoming OSC messages
//
//  * holds a list of actors it connects the values to
//************************************************************

class UdpInput: public ListButton
{

public:

    //************************************************************
    //Variables
    //************************************************************

    Pilot *                             interpreter;                    //the interpreter we use to parse the OSC messages
    UdpTransmitSocket *                 transmitSocket;                 // UDP Socket for transmitting data - only used for debugging purposes
	UdpListeningReceiveSocket *         listenSocket;                   // UDP Socket for listening to messages
    std::vector <InputConnectButton*>   inputConnectButtons;            //InputConnectButtons are created from within the pilot for each set of data it receives
                                                                        //representations of the values received - also used for connecting to properties

    std::vector <string>                targetProperties;               //holds the properties to be changed
    std::vector <string>                targetValues;                   //holds the properties to be changed
    std::vector <Vector4f>              targetMin;                    //holds the range of values - is an array of 8 to go up to a max of Vector4f
    std::vector <Vector4f>              targetMax;                    //holds the range of values - is an array of 8 to go up to a max of Vector4f

    std::vector <Actor*>                targetActors;                   //list of Actors we are controlling from this input

    std::string                         pilotType;                      //determines the type of message interpreter to create

    int                                 channel;                        //UDPInput is handlesd on port 31840+channel

#ifdef TARGET_WIN32
    HANDLE mutex;                                                       //the MUTEX for the pilot thread
#endif

#ifdef TARGET_MACOSX
	pthread_mutex_t mutex;
	pthread_t udpThread;
#endif

#ifdef TARGET_LINUX
	pthread_mutex_t mutex;
	pthread_t udpThread;
#endif

    //************************************************************
    //Constructor/Destructor
    //************************************************************

    UdpInput();
    ~UdpInput();


    //************************************************************
    //Functions
    //************************************************************

    virtual void setup();

    virtual void update(double deltaTime);                                              //updates the links between actors and incoming data

    void threadedFunctionCall(void *);                                  //calls a static function to start the thread


    virtual void mouseDrag();
    virtual void finishDrag();
    virtual void clickedLeft();

    virtual void trigger(MsbObject* other);

    virtual void registerProperties();

    virtual void create();


    //for visual representation of buttons
    virtual void createNewInputConnect(Vector4f btnColor, std::string btnName, unsigned int numInputs);

#ifdef TARGET_WIN32
    //static threading call that will call the object-threading, similar to the singleton pattern, but different...
    static unsigned __stdcall ThreadStaticEntryPoint(void * pThis){
#endif

#ifdef TARGET_MACOSX
	static void *ThreadStaticEntryPoint(void * pThis){
#endif

#ifdef TARGET_LINUX
	static void *ThreadStaticEntryPoint(void * pThis){
#endif


      UdpInput * pthX = (UdpInput*)pThis;   // the tricky cast

      cout << "running thread for:" << pthX->name <<endl;
      pthX->threadedFunctionCall(NULL);    // now call the true entry-point-function

      // A thread terminates automatically if it completes execution,
      // or it can terminate itself with a call to _endthread().
#ifdef TARGET_WIN32
	  return 1;          // the thread exit code
#endif
		return 0;
    }

/*
    virtual void load(TiXmlElement *myInfo);         //loads all properties as defined in registerProperties

    virtual TiXmlElement* save(TiXmlElement *root);          //saves all properties and inputConnectButton thingys
*/
};
#endif
