#include "pilot.h"
#include "udpInput.h"
#include "input.h"
#include "sceneData.h"
#include "cameraActor.h"

Pilot::Pilot(){

input=Input::getInstance();
sceneData=SceneData::getInstance();
renderer=Renderer::getInstance();
parent=NULL;
mutex=NULL;

name="Pilot";

}

void Pilot::ProcessMessage( const osc::ReceivedMessage& m,const IpEndpointName& remoteEndpoint ){


	    try{
            // example of parsing single messages. osc::OsckPacketListener
            // handles the bundle traversal.


              char* myAdress=(char*)m.AddressPattern();
              char* adressPart = strtok(myAdress, "/"); // Splits spaces between words in st

              //cout << "got a new message!" << endl;
              //cout << adressPart << endl;

              //seems to be necessary for comparison?
              std::string partAsString=adressPart;

            ///TODO: These should be put in a more general thing called /command/...
             if (partAsString=="trigger"){
                triggerPart(m,adressPart,partAsString);
			 }

             if (partAsString=="play"){
                playPart(m,adressPart,partAsString);
			 }

             if (partAsString=="switchCameraToSelected"){
                switchCameraToSelected(m,adressPart,partAsString);
			 }

             if (partAsString=="selectActorByName"){
                selectPart(m,adressPart,partAsString);
			 }

             if (partAsString=="setPropertyForSelected"){
                setPropertyForSelected(m,adressPart,partAsString);
			 }

             if (partAsString=="switchMonitor"){
                renderer->monitorNumber=1;
			 }



            ///Control Brush directly!

              if (partAsString=="brush"){
                    brushPart(m,adressPart,partAsString);
              }



            if (partAsString=="pilot"){
                pilotPart(m, adressPart, partAsString);
            }
        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }

}

void Pilot::setPropertyForSelected(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

               cout << "received setPropertyForSelected..." << endl;

               if (sceneData->selectedActors.size()<1){
                    cout << "no actor selected..." << endl;
                    return;
               }


                //and once more, so we move forward
                adressPart = strtok(NULL, "/");

                string propertyName="NULL";

                //get pointer to al the arguments
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

                //counter for all inputs
                unsigned int numInputs=0;

                //Get MUTEX
#ifdef TARGET_WIN32
                WaitForSingleObject( *mutex, INFINITE );
#endif
#ifdef TARGET_MACOSX
				pthread_mutex_lock(mutex);
#endif
					while(adressPart!=NULL){

                    partAsString=adressPart;


                    if (partAsString=="string"){

                    propertyName=(arg++)->AsString();

                    }
                    else if (partAsString=="matrix4f"){
                        Matrix4f matrixContainer;

                        for (int i=0; i<16;i++){
                            //cout << "transfering dataPoint: " << i << " at frame: " << renderer->frames << endl;
                            matrixContainer.data[i]=(arg++)->AsFloat();
                            }

                         char value[64];

                            //update value
                            //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                            //we could create a faster Pilot that transforms locations directly as a subclass...
                            sprintf(value,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
                                                matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2], matrixContainer.data[3],
                                                matrixContainer.data[4], matrixContainer.data[5], matrixContainer.data[6], matrixContainer.data[7],
                                                matrixContainer.data[8], matrixContainer.data[9], matrixContainer.data[10],matrixContainer.data[11],
                                                matrixContainer.data[12],matrixContainer.data[13],matrixContainer.data[14],matrixContainer.data[15]);

                            string valueAsString=value;

                            memberID* myMemberID=&sceneData->selectedActors[0]->property[propertyName];
                            sceneData->selectedActors[0]->memberFromString(myMemberID,valueAsString);
                        }

                        else if (partAsString=="matrix3f"){

                            Matrix3f matrixContainer;

                            for (int i=0; i<9;i++){
                                matrixContainer.data[i]=(arg++)->AsFloat();
                                }

                                char value[64];


                                //visual representation ->mapping the location part of the matrix to visuals
                                Vector3f matrixLoc=Vector3f(matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2]);
                                parent->inputConnectButtons[numInputs]->color=Vector4f(matrixLoc.x,matrixLoc.y,matrixLoc.z,parent->inputConnectButtons[numInputs]->color.a);

                                //update value
                                //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                                //we could create a faster Pilot that transforms locations directly as a subclass...
                                sprintf(value,"mat3f %f %f %f %f %f %f %f %f %f",
                                                    matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2],
                                                    matrixContainer.data[3], matrixContainer.data[4], matrixContainer.data[5],
                                                    matrixContainer.data[6], matrixContainer.data[7], matrixContainer.data[8]);

                            string valueAsString=value;

                            memberID* myMemberID=&sceneData->selectedActors[0]->property[propertyName];
                            sceneData->selectedActors[0]->memberFromString(myMemberID,valueAsString);
                        }



                    else if (partAsString=="vector3f" ){

                        float x=0.0;
                        float y=0.0;
                        float z=0.0;

                        Vector3f vectorContainer= Vector3f (0,0,0);
                        x=(arg++)->AsFloat();
                        y=(arg++)->AsFloat();
                        z=(arg++)->AsFloat();

                            char value[64];

                            //update value
                            //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                            //we could create a faster Pilot that transforms locations directly as a subclass...
                            sprintf(value,"vec3f %f %f %f",vectorContainer.x, vectorContainer.y, vectorContainer.z);

                            string valueAsString=value;
                            memberID* myMemberID=&sceneData->selectedActors[0]->property[propertyName];
                            sceneData->selectedActors[0]->memberFromString(myMemberID,valueAsString);

                      }

                    else if (partAsString=="float"){
                          char value[64] ;
                          float number=(arg++)->AsFloat();


                          //update value
                          //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                          //we could create a faster Pilot that transforms locations directly as a subclass...
                          sprintf(value,"float %f",number);
                            string valueAsString=value;
                            memberID* myMemberID=&sceneData->selectedActors[0]->property[propertyName];
                            sceneData->selectedActors[0]->memberFromString(myMemberID,valueAsString);
                      }

                    adressPart = strtok(NULL, "/");
					}
#ifdef TARGET_WIN32
                ReleaseMutex( *mutex );
#endif
#ifdef TARGET_MACOSX
				pthread_mutex_unlock(mutex);
#endif

}



void Pilot::switchCameraToSelected(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

    osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();


    //Get MUTEX
#ifdef TARGET_WIN32
     WaitForSingleObject( *mutex, INFINITE );
#endif
#ifdef TARGET_MACOSX
     pthread_mutex_lock(mutex);
#endif

     if (sceneData->selectedActors.size()<1){
                    cout << "no actor selected..." << endl;
                    return;
               }

    sceneData->controller->controlledActor=sceneData->selectedActors[0];

    //sceneData->controller->switchTool(TOOL_NAV);
    sceneData->updateView();

    //TODO: for depth of field and FOV
    //CameraActor* cA=dynamic_cast<CameraActor*>(sceneData->selectedActors[0]);

#ifdef TARGET_WIN32
    ReleaseMutex( *mutex );
#endif
#ifdef TARGET_MACOSX
     pthread_mutex_unlock(mutex);
#endif
}


void Pilot::selectPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

    osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();


                string s=(arg)->AsString();

                cout << "received select event is: " << s << endl;

     //Get MUTEX
#ifdef TARGET_WIN32
     WaitForSingleObject( *mutex, INFINITE );
#endif
#ifdef TARGET_MACOSX
     pthread_mutex_lock(mutex);
#endif

                input->deselectActors();

                for (int i=0;i<(int)sceneData->actorList.size();i++){
                    if (sceneData->actorList[i]->name==s){
                        sceneData->actorList[i]->bSelected=true;
                        sceneData->selectedActors.push_back(sceneData->actorList[i]);
                    }
                }


#ifdef TARGET_WIN32
    ReleaseMutex( *mutex );
#endif
#ifdef TARGET_MACOSX
     pthread_mutex_unlock(mutex);
#endif

}


void Pilot::playPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

                 //Get MUTEX
#ifdef TARGET_WIN32
				 WaitForSingleObject( *mutex, INFINITE );
#endif
#ifdef TARGET_MACOSX
				 pthread_mutex_lock(mutex);
#endif
				 char str[16];
                sprintf(str,"%i", parent->channel);
                sceneData->eventTrigger.push_back(str);

#ifdef TARGET_WIN32
                ReleaseMutex( *mutex );
#endif
#ifdef TARGET_MACOSX
				 pthread_mutex_unlock(mutex);
#endif
}

void Pilot::triggerPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

                 osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();


                string event=(arg)->AsString();

                cout << "received trigger! event is: " << event << endl;

                 //Get MUTEX
#ifdef TARGET_WIN32
				 WaitForSingleObject( *mutex, INFINITE );
#endif
#ifdef TARGET_MACOSX
				 pthread_mutex_lock(mutex);
#endif
                sceneData->eventTrigger.push_back(event);

#ifdef TARGET_WIN32
                ReleaseMutex( *mutex );
#endif
#ifdef TARGET_MACOSX
				 pthread_mutex_unlock(mutex);
#endif
}

void Pilot::brushPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

                adressPart = strtok(NULL, "/");

                //get pointer to al the arguments
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

                //Get MUTEX
                #ifdef TARGET_WIN32
                WaitForSingleObject( *mutex, INFINITE );
                #endif
                #ifdef TARGET_MACOSX
                pthread_mutex_lock(mutex);
                #endif
                while(adressPart!=NULL){

                    partAsString=adressPart;

                    if (partAsString=="vector3f" ){

                        cout << "first Argument: " << (arg)->AsFloat() << endl;
                        float x=0.0;
                        float y=0.0;
                        float z=0.0;

                        x=(arg++)->AsFloat();
                        y=(arg++)->AsFloat();
                        z=(arg++)->AsFloat();

                        sceneData->brush->setLocation(Vector3f(x,y,z));

                      }//end if vector3f


                      else if (partAsString=="new"){

                        if (sceneData->controller->currentTool != sceneData->drawTool){
                            sceneData->controller->switchTool(TOOL_DRAW);
                        }

                        sceneData->drawTool->bMouseControlled=false;
                        sceneData->drawTool->createNewDrawing();
                      }

                      else if (partAsString=="draw"){

                        if (sceneData->controller->currentTool != sceneData->drawTool){
                            sceneData->controller->switchTool(TOOL_DRAW);
                        }

                        //TODO: hack to draw without mouse movement
                        input->mouseVector=Vector3f(1,1,1);


                        sceneData->drawTool->paint();
                      }
                //move to next part in address
                adressPart = strtok(NULL, "/");
                }//end while

                #ifdef TARGET_WIN32
                ReleaseMutex( *mutex );
                #endif
                #ifdef TARGET_MACOSX
				 pthread_mutex_unlock(mutex);
                #endif
}

void Pilot::pilotPart(const osc::ReceivedMessage& m, char* adressPart, string partAsString){

                //cout << "got pilot!" << endl;

                //and once more, so we move forward
                adressPart = strtok(NULL, "/");

                //get pointer to al the arguments
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

                //counter for all inputs
                unsigned int numInputs=0;

                //Get MUTEX
#ifdef TARGET_WIN32
                WaitForSingleObject( *mutex, INFINITE );
#endif
#ifdef TARGET_MACOSX
				pthread_mutex_lock(mutex);
#endif
					while(adressPart!=NULL){

                    partAsString=adressPart;


                    //if not already existing: create new inputConnectButton in UdpInput
                    //if existing: check for linking info and delegate data to linking info
                    if (partAsString=="matrix4f"){
                        Matrix4f matrixContainer;

                        for (int i=0; i<16;i++){
                            //cout << "transfering dataPoint: " << i << " at frame: " << renderer->frames << endl;
                            matrixContainer.data[i]=(arg++)->AsFloat();
                            }

                        //cout << matrixContainer << endl;

                        if (parent->inputConnectButtons.size()==numInputs){
                          parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,0.25),"vector3f", numInputs);
                          }
                        //we already have a button ->this is the actual update
                        else{
                            char value[64];


                            //visual representation ->mapping the x-Axis part of the matrix to visuals
                            Vector3f matrixLoc=matrixContainer.getTranslation();
                            parent->inputConnectButtons[numInputs]->color=Vector4f(matrixLoc.x,matrixLoc.y,matrixLoc.z,parent->inputConnectButtons[numInputs]->color.a);

                            //update value
                            //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                            //we could create a faster Pilot that transforms locations directly as a subclass...
                            sprintf(value,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
                                                matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2], matrixContainer.data[3],
                                                matrixContainer.data[4], matrixContainer.data[5], matrixContainer.data[6], matrixContainer.data[7],
                                                matrixContainer.data[8], matrixContainer.data[9], matrixContainer.data[10],matrixContainer.data[11],
                                                matrixContainer.data[12],matrixContainer.data[13],matrixContainer.data[14],matrixContainer.data[15]);

                            parent->targetValues[numInputs]=value;

                            //cout << "converted a Matrix: " << value << " for input: " << numInputs << endl;
                            }
                        }

                        else if (partAsString=="matrix3f"){

                                //cout << "found rotation matrix!" << endl;

                            Matrix3f matrixContainer;

                            for (int i=0; i<9;i++){
                                matrixContainer.data[i]=(arg++)->AsFloat();
                                }

                            //cout << matrixContainer << endl;

                            if (parent->inputConnectButtons.size()==numInputs){
                              parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,0.25),"vector3f", numInputs);
                              }
                            //we already have a button ->this is the actual update
                            else{
                                char value[64];


                                //visual representation ->mapping the location part of the matrix to visuals
                                Vector3f matrixLoc=Vector3f(matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2]);
                                parent->inputConnectButtons[numInputs]->color=Vector4f(matrixLoc.x,matrixLoc.y,matrixLoc.z,parent->inputConnectButtons[numInputs]->color.a);

                                //update value
                                //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                                //we could create a faster Pilot that transforms locations directly as a subclass...
                                sprintf(value,"mat3f %f %f %f %f %f %f %f %f %f",
                                                    matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2],
                                                    matrixContainer.data[3], matrixContainer.data[4], matrixContainer.data[5],
                                                    matrixContainer.data[6], matrixContainer.data[7], matrixContainer.data[8]);

                                parent->targetValues[numInputs]=value;

                                //cout << "converted a Matrix: " << value << " for input: " << numInputs << endl;
                                }
                        }



                    else if (partAsString=="vector3f" ){

                        //cout << "first Argument: " << (arg)->AsFloat() << endl;
                        float x=0.0;
                        float y=0.0;
                        float z=0.0;

                        Vector3f vectorContainer= Vector3f (0,0,0);
                        x=(arg++)->AsFloat();
                        y=(arg++)->AsFloat();
                        z=(arg++)->AsFloat();

                        //cout << vectorContainer << endl;
                        //encountered a new input -> we need to create a button for this!
                        if (parent->inputConnectButtons.size()==numInputs){
                          parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,0.25),"vec3f", numInputs);
                          }
                        //we already have a button ->this is the actual update
                        else{
                            char value[64];


                            //visual representation
                            parent->inputConnectButtons[numInputs]->color=Vector4f(x,y,z,parent->inputConnectButtons[numInputs]->color.a);

                            //map values
                            if (parent->targetMin.size()>numInputs && parent->targetMin[numInputs]!=NULL){

                                vectorContainer.x=sceneData->setToRange(parent->targetMin[numInputs].x,parent->targetMax[numInputs].x, x);
                                vectorContainer.y=sceneData->setToRange(parent->targetMin[numInputs].y,parent->targetMax[numInputs].y, y);
                                vectorContainer.z=sceneData->setToRange(parent->targetMin[numInputs].z,parent->targetMax[numInputs].z, z);
                                //cout << "mapping the values to " << parent->targetRange[numInputs] << endl;
                            }
                            else
                                cout << "something's wrong with the targetMin/Max stuff!" << endl;

                            //update value
                            //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                            //we could create a faster Pilot that transforms locations directly as a subclass...
                            sprintf(value,"vec3f %f %f %f",vectorContainer.x, vectorContainer.y, vectorContainer.z);
                            parent->targetValues[numInputs]=value;

                            //cout << "converted a Vector: " << value << " for input: " << numInputs << endl;
                            }

                      }

                      else if (partAsString=="vector4f" ){

                        //cout << "first Argument: " << (arg)->AsFloat() << endl;
                        float x=0.0;
                        float y=0.0;
                        float z=0.0;
                        float w=0.0;

                        Vector4f vectorContainer= Vector4f (0,0,0,0);
                        x=(arg++)->AsFloat();
                        y=(arg++)->AsFloat();
                        z=(arg++)->AsFloat();
                        w=(arg++)->AsFloat();

                        //cout << vectorContainer << endl;
                        //encountered a new input -> we need to create a button for this!
                        if (parent->inputConnectButtons.size()==numInputs){
                          parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,0.25),"vec4f", numInputs);
                          }
                        //we already have a button ->this is the actual update
                        else{
                            char value[64];


                            //visual representation
                            parent->inputConnectButtons[numInputs]->color=Vector4f(x,y,z,parent->inputConnectButtons[numInputs]->color.a);

                            //map values
                            if (parent->targetMin.size()>numInputs && parent->targetMin[numInputs]!=NULL){

                                vectorContainer.x=sceneData->setToRange(parent->targetMin[numInputs].x,parent->targetMax[numInputs].x, x);
                                vectorContainer.y=sceneData->setToRange(parent->targetMin[numInputs].y,parent->targetMax[numInputs].y, y);
                                vectorContainer.z=sceneData->setToRange(parent->targetMin[numInputs].z,parent->targetMax[numInputs].z, z);
                                vectorContainer.w=sceneData->setToRange(parent->targetMin[numInputs].w,parent->targetMax[numInputs].w, w);
                                //cout << "mapping the values to " << parent->targetRange[numInputs] << endl;
                            }
                            else
                                cout << "something's wrong with the targetMin/Max stuff!" << endl;

                            //update value
                            //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                            //we could create a faster Pilot that transforms locations directly as a subclass...
                            sprintf(value,"vec4f %f %f %f %f",vectorContainer.x, vectorContainer.y, vectorContainer.z, vectorContainer.w);
                            parent->targetValues[numInputs]=value;

                            //cout << "converted a Vector: " << value << " for input: " << numInputs << endl;
                            }

                      }

                    else if (partAsString=="float"){
                        if (parent->inputConnectButtons.size()==numInputs)                          {
                          parent->createNewInputConnect(Vector4f(0.3,1.0,0.3,0.25),"float", numInputs);
                          arg++;
                          }
                        else{
                          char value[64] ;
                          float number=(arg++)->AsFloat();

                          //cout << number << endl;
                          //update color of button for visual representation
                          parent->inputConnectButtons[numInputs]->color=Vector4f(number,number,number,parent->inputConnectButtons[numInputs]->color.a);

                          //map values
                          number=sceneData->setToRange(parent->targetMin[numInputs].x,parent->targetMax[numInputs].x,number);

                          //update value
                          //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                          //we could create a faster Pilot that transforms locations directly as a subclass...
                          sprintf(value,"float %f",number);
                          parent->targetValues[numInputs]=value;
                          }
                      }

                    else if (partAsString == "kinectSkeleton"){
                        if (parent->inputConnectButtons.size()==numInputs)                          {
                          parent->createNewInputConnect(Vector4f(0.8,1.0,0.2,0.25),"KinectSkeleton", numInputs);
                          //no arguments attached! Do not skip args!
                          //arg++;
                          }
                         else{
                            //update value
                            //not sure if we need a value here! This is just bogus!
                            parent->targetValues[numInputs]="string NULL";
                        }

                      }
                adressPart = strtok(NULL, "/");
                numInputs++;
                }

#ifdef TARGET_WIN32
                ReleaseMutex( *mutex );
#endif
#ifdef TARGET_MACOSX
				pthread_mutex_unlock(mutex);
#endif

}

void Pilot::update(float deltaTime){

}
