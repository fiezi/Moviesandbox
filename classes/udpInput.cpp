//TODO: this is windows only land!

#include "udpInput.h"
#include "input.h"


UdpInput::UdpInput(){

bAlwaysUpdate=true;
bPermanent=true;
bDragable=true;
sceneShaderID="color";
scale.x=60;
scale.y=30;
color=Vector4f(0.3,0.7,0.3,1.0);
channel=0;
//clear memory

listenSocket=NULL;

transmitSocket=NULL;
interpreter=NULL;

bTextured=true;
pilotType="pilot";

#ifdef TARGET_WIN32
	mutex= CreateMutex( NULL,FALSE,NULL );
#endif

#ifdef TARGET_MACOSX
//	mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

level=1;
bDrawName=true;
listWidth=60;

listType.push_back("15TextInputButton");
listName.push_back("name");
listProp.push_back("NAME");
listIcon.push_back("icon_base");

listType.push_back("15TextInputButton");
listName.push_back("channel");
listProp.push_back("CHANNEL");
listIcon.push_back("icon_base");

registerProperties();
}

UdpInput::~UdpInput(){

    cout << "destroying UDPInput!" << endl;
    //TODO: close Socket and stuff!
    //also destroy inputConnectButons!
    for (int i=0;i<(int)inputConnectButtons.size();i++){
        inputConnectButtons[i]->remove();
    }
    inputConnectButtons.clear();

    delete(listenSocket);
}

void UdpInput::registerProperties(){

ListButton::registerProperties();
createMemberID("CHANNEL",&channel,this);
createMemberID("PILOTTYPE",&pilotType,this);

createMemberID("TARGETPROPERTIES",&targetProperties,this);
createMemberID("TARGETMIN",&targetMin,this);
createMemberID("TARGETMAX",&targetMax,this);
createMemberID("TARGETACTORS",&targetActors,this);

}

void UdpInput::setup(){

    ListButton::setup();


    //here you will be able to choose from different Pilots! Hopefully...
    //TODO: needs a bit more work to make this easier/automated (with "create" method)
    if (pilotType=="multiTouchPilot")
      {
      //interpreter=new MultiTouchPilot;
      //cout << "created a multiTouch thingy" << endl;
      }
    else
      {
      interpreter=new Pilot;
      cout << "created a simple pilot" << endl;
      }

    //setup pilot for threading

    interpreter->mutex=&mutex;
    interpreter->name=name;
    interpreter->parent=this;

    //TODO: transmit in UDPInput only used for testing
    //transmitSocket=new UdpTransmitSocket ( IpEndpointName( "127.0.0.1", 31840+channel ) );


    //kinda fake, but will work. Should find a better way of knowing if port is bound...
    bool bChannelFree=true;

    for (int i=0;i<(int)renderer->buttonList.size();i++){
        if (renderer->buttonList[i]->isA("UdpInput")){
            UdpInput* myUdpInput=(UdpInput*)renderer->buttonList[i];
            if (myUdpInput->channel==channel && myUdpInput!=this)
                bChannelFree=false;
            }
    }

    if (bChannelFree){
        listenSocket = new UdpListeningReceiveSocket(IpEndpointName( IpEndpointName::ANY_ADDRESS, 31840+channel ),interpreter );

        //create a thread by calling a static function with a void function pointer
#ifdef TARGET_WIN32
			HANDLE   hth1;

            hth1 = (HANDLE)_beginthreadex( NULL,0,ThreadStaticEntryPoint,(void *)this,0,NULL);

            if ( hth1 == 0 )
                printf("Failed to create thread 1\n");
#endif
#ifdef TARGET_MACOSX
		int err=0;
		err = pthread_create( &udpThread, NULL, ThreadStaticEntryPoint, (void *)this);
#endif

    }else{
        cout << "couldn't bind to port because port was already taken!" << endl;
    }


}

//Here we assign the value we get from the interpreter!
void UdpInput::update(double deltaTime){


    for (int i=0;i<(int)inputConnectButtons.size();i++){
        inputConnectButtons[i]->setLocation(location + Vector3f(0, scale.y+5+ i*20, 0) );


        }

    for (unsigned int i=0; i<targetValues.size();i++){
        if (targetActors[i]!=NULL &&
            targetProperties[i]!="NULL" &&
            targetValues[i]!="NULL"){

                memberID* mID = &(targetActors[i]->property[targetProperties[i]]);

                //cout << "property " << targetProperties[i] << " in " << targetActors[i]->name << endl;

                //cout << "targetValue: " << targetValues[i] << endl;

                //need special handling for location and rotation so we can move the ragdoll or other complex systems
                //also, strings only really compare well with other strings, thus the somewhat odd variable declaration...
                string locationString="LOCATION";
                string rotationString="ROTATION";
                if (!mID){
                    cout << "WAAAAAAAHHHHHHH..." << endl;
                    return;
                }

                if (mID->memberName==locationString){
                    targetActors[i]->setLocation(readVector3f((char*)targetValues[i].c_str()));
                }
                else if (mID->memberName==rotationString){
                    //check if we're having a vector
                    if( strncmp("vec3f ",(char*)targetValues[i].c_str(),6) == 0 ){
                        targetActors[i]->setRotation(readVector3f((char*)targetValues[i].c_str()));
                    }
                    //otherwise interpret as matrix!
                    else{
                        targetActors[i]->transformMatrix.setRotation(readMatrix3f((char*)targetValues[i].c_str()));
                        if (targetActors[i]->bDebug && input->controller->bRunning)
                            cout << "setting UDPInput rotation now..." << renderer->frames <<endl;
                    }
                }
                //this is the standard way of assigning properties
                else{
                    memberFromString(mID,targetValues[i]);
                    //cout << "yay!" << endl;
                    //cout << targetActors[i]->name << endl;
                }
        }
    }
}

void UdpInput::mouseDrag(){

input->dragButton=this;
setLocation(Vector3f(input->mouseX,input->mouseY,0));
}

void UdpInput::finishDrag(){

input->dragButton=NULL;
}

void UdpInput::clickedLeft(){

    ListButton::clickedLeft();

/*
    char buffer[1024];
    osc::OutboundPacketStream p( buffer, 1024 );

    p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/test1" )
            << true << 23 << (float)3.1415 << "hello" << osc::EndMessage
        << osc::BeginMessage( "/test2" )
            << true << 24 << (float)10.8 << "world" << osc::EndMessage
        << osc::EndBundle;

    transmitSocket->Send( p.Data(), p.Size() );
*/
}


//Threading function so we create a thread per UdpInput Button Instance
void UdpInput::threadedFunctionCall (void *){
   listenSocket->Run();
}


//create new inputConnectButton, targetActor Reference and
void UdpInput::createNewInputConnect(Vector4f btnColor, std::string btnName, unsigned int numInputs){

    InputConnectButton* myBtn=new InputConnectButton;
    myBtn->setLocation(location + Vector3f(0, scale.y+5+ numInputs*20, 0) );
    myBtn->color=btnColor;
    myBtn->name=btnName;
    //connect directly to UDPInput - but why?
    myBtn->parent=this;
    myBtn->listPosition=numInputs;
    renderer->buttonList.push_back(myBtn);
    inputConnectButtons.push_back(myBtn);
    myBtn->setup();


    //create a field for the actor target reference - use NULL
    if (numInputs>=targetActors.size())
        targetActors.push_back(NULL);

    if (numInputs>=targetProperties.size())
        targetProperties.push_back("NULL");

    if (numInputs>=targetValues.size())
        targetValues.push_back("NULL");

    if (numInputs>=targetMin.size()){
        targetMin.push_back(Vector4f(0,0,0,0));
        targetMax.push_back(Vector4f(1,1,1,1));
        }

}

void UdpInput::create(){renderer->addButton(this);}



void UdpInput::trigger(MsbObject* other){


    if (listenSocket){
        listenSocket->Break();
	}
    listenSocket = new UdpListeningReceiveSocket(IpEndpointName( IpEndpointName::ANY_ADDRESS, 31840+channel ),interpreter );

    //create a thread by calling a static function with a void function pointer
#ifdef TARGET_WIN32

		HANDLE   hth1;

        hth1 = (HANDLE)_beginthreadex( NULL,0,ThreadStaticEntryPoint,(void *)this,0,NULL);

        if ( hth1 == 0 )
            printf("Failed to create thread 1\n");
#endif
#ifdef TARGET_MACOSX
	int err=0;
	err = pthread_create( &udpThread, NULL, ThreadStaticEntryPoint, (void *)this);
	if (err==0)
		cout << "error creating thread!" << endl;
#endif

}
