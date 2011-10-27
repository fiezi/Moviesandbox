#include "pilot.h"
#include "udpInput.h"
#include "input.h"
#include "sceneData.h"

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

             if (partAsString=="trigger"){
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

             if (partAsString=="play"){
                 osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

                //float event=(arg)->AsInt32();
                //cout << "received trigger! event is: " << event << endl;

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


            ///Control Brush directly!

              if (partAsString=="brush"){

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

            }//end /brush



              if (partAsString=="pilot")
                {

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
                          parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,1.0),"vector3f", numInputs);
                          }
                        //we already have a button ->this is the actual update
                        else{
                            char value[64];


                            //visual representation ->mapping the location part of the matrix to visuals
                            Vector3f matrixLoc=matrixContainer.getTranslation();
                            parent->inputConnectButtons[numInputs]->color=Vector4f(matrixLoc.x,matrixLoc.y,matrixLoc.z,1.0);

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
                          parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,1.0),"vector3f", numInputs);
                          }
                        //we already have a button ->this is the actual update
                        else{
                            char value[64];


                            //visual representation ->mapping the location part of the matrix to visuals
                            Vector3f matrixLoc=Vector3f(matrixContainer.data[0], matrixContainer.data[1], matrixContainer.data[2]);
                            parent->inputConnectButtons[numInputs]->color=Vector4f(matrixLoc.x,matrixLoc.y,matrixLoc.z,1.0);

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
                          parent->createNewInputConnect(Vector4f(1.0,0.3,0.3,1.0),"vector3f", numInputs);
                          }
                        //we already have a button ->this is the actual update
                        else{
                            char value[64];


                            //visual representation
                            parent->inputConnectButtons[numInputs]->color=Vector4f(x,y,z,1.0);

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

                    else if (partAsString=="float"){
                        if (parent->inputConnectButtons.size()==numInputs)                          {
                          parent->createNewInputConnect(Vector4f(0.3,1.0,0.3,1.0),"float", numInputs);
                          arg++;
                          }
                        else{
                          char value[64] ;
                          float number=(arg++)->AsFloat();

                          //cout << number << endl;
                          //update color of button for visual representation
                          parent->inputConnectButtons[numInputs]->color=Vector4f(number,number,number,1.0);

                          //map values
                          number=sceneData->setToRange(parent->targetMin[numInputs].x,parent->targetMax[numInputs].x,number);

                          //update value
                          //here we convert back into a string, which is slow, but very versatile, as it can be assigned to any property of any actor if it fits its type.
                          //we could create a faster Pilot that transforms locations directly as a subclass...
                          sprintf(value,"float %f",number);
                          parent->targetValues[numInputs]=value;
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
        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }

}

void Pilot::update(float deltaTime){

}
