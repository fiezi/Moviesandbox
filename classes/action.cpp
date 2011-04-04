

#include "action.h"
#include "renderer.h"
#include "input.h"
#include "interpolationHelper.h"
#include "performActionNode.h"
#include "timelineButton.h"
#include "rootNode.h"

Action::Action(){

        bAlwaysUpdate=true;
        bPermanent=true;
        bHidden=true;
        bDragable=true;
        scale.x=48;
        scale.y=48;
        bTextured=true;
        textureID="icon_base";
        name="newAction";
        color=Vector4f(1.0,1.0,0.0,1.0);

        inPoint=0.0;
        outPoint=0.0;
        timeScale=1.0;

        bSkeletal=false;
        bRelative=false;


        listType.push_back("15TextInputButton");
        listName.push_back("name");
        listProp.push_back("NAME");
        listIcon.push_back("icon_base");

        listType.push_back("15TextInputButton");
        listName.push_back("timeScale");
        listProp.push_back("TIMESCALE");
        listIcon.push_back("icon_base");

        listType.push_back("15TextInputButton");
        listName.push_back("relative");
        listProp.push_back("BRELATIVE");
        listIcon.push_back("icon_base");

        listType.push_back("15TextInputButton");
        listName.push_back("skeletal");
        listProp.push_back("BSKELETAL");
        listIcon.push_back("icon_base");


        listType.push_back("15TextInputButton");
        listName.push_back("import");
        listProp.push_back("NULL");
        listIcon.push_back("icon_load");

        registerProperties();
}

Action::~Action(){}

void Action::registerProperties(){

        BasicButton::registerProperties();
        createMemberID("TIMESCALE",&timeScale,this);
        createMemberID("BRELATIVE",&bRelative,this);
        createMemberID("BSKELETAL",&bSkeletal,this);
}

void Action::setup(){
    //TODO: quick fix for saving bHidden=false in actions!
    bHidden=true;
    ListButton::setup();
}

void Action::update(double deltaTime){
}

void Action::mouseOver(){
    BasicButton::mouseOver();
}

void Action::mouseDrag(){

    input->dragButton=this;
    setLocation(Vector3f(input->mouseX, input->mouseY,0));

}

void Action::finishDrag(){


    if (input->worldTarget && input->worldTarget->name!="ground" && input->worldTarget->baseNode){
        Node*   myRoot=NULL;
        Node*   myNode=NULL;
        myRoot=input->worldTarget->baseNode;
        //crawl down the node tree so that we append our action to the last node in the tree

        if (myRoot)
            while (myRoot->childNode){
                myRoot=myRoot->childNode;
                }

        sceneData->actorInfo["17PerformActionNode"].actorReference->create();
        myNode=sceneData->nodeList.back();

        myNode->setLocation(Vector3f( myRoot->location.x, myRoot->location.y+50, 0 ));
        myRoot->connectChild(myNode);
        myNode->setup();
        ((PerformActionNode*)myNode)->bRelative=bRelative;
        ((PerformActionNode*)myNode)->applyTo=input->worldTarget;
        ((PerformActionNode*)myNode)->myAction=name;



        }
    setLocation(initialLocation);

    TimelineButton* tlb=NULL;
    tlb=dynamic_cast<TimelineButton*>(input->hudTarget);
    if( input->hudTarget && tlb )
    {
       ((TimelineButton*)input->hudTarget)->populateFromAction(this);
    }



}

void Action::clickedLeft(){
    BasicButton::clickedLeft();
}

void Action::clickedRight(){
    ListButton::clickedLeft();
}

void Action::focusClick(){

}

void Action::deselect(int depth){

ListButton::deselect(depth);
}


void Action::trigger(MsbObject* other){

    if(other->name=="import"){
        importAction(input->inputText);
    }

}


void Action::importAction(string filename){


    string stringName="resources/actions/";
    stringName.append(filename);

    TiXmlDocument doc( stringName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Make sure this is a Moviesandbox file!
    //***********************************************************************

    element=hDoc.FirstChildElement().Element();
    if (!element) return;

    //keep master key for bone references and stuff
    key* masterKey=keyFrames[0];

    //clear all keys except our master key;
    for (int i=1;i<(int)keyFrames.size();i++){
        delete(keyFrames[i]);
    }
    keyFrames.clear();

    hRoot=TiXmlHandle(element);

    string content;
    int frame;


    //read info from characterRecorder file
    element=hRoot.FirstChild( "skeleton" ).Element();
    while (element){
        TiXmlElement* timeElement=element->FirstChildElement("id");
        sscanf(timeElement->GetText(),"%u", &frame);
        cout << "frame: "<< frame << endl;


        TiXmlElement* boneElement=element->FirstChildElement("neckRot");
        content=boneElement->GetText();
        Matrix3f neckRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("torsoRot");
        content=boneElement->GetText();
        Matrix3f torsoRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("pelvisRot");
        content=boneElement->GetText();
        Matrix3f pelvisRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("leftShoulderRot");
        content=boneElement->GetText();
        Matrix3f leftShoulderRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("rightShoulderRot");
        content=boneElement->GetText();
        Matrix3f rightShoulderRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("rightElbowRot");
        content=boneElement->GetText();
        Matrix3f rightElbowRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("leftElbowRot");
        content=boneElement->GetText();
        Matrix3f leftElbowRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("leftHipRot");
        content=boneElement->GetText();
        Matrix3f leftHipRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("leftKneeRot");
        content=boneElement->GetText();
        Matrix3f leftKneeRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("rightHipRot");
        content=boneElement->GetText();
        Matrix3f rightHipRot=readMatrix3f((char*)content.c_str());

        boneElement=element->FirstChildElement("rightKneeRot");
        content=boneElement->GetText();
        Matrix3f rightKneeRot=readMatrix3f((char*)content.c_str());



        //now construct key
        key* currentKey=new key;
        currentKey->timeKey=((float)frame) /12.0f;
        currentKey->timeKey*=1000;     //milliseconds!
        //set bone matrices
        /*
        currentKey->boneMatrices["neck"]=setRotationComponent(masterKey->boneMatrices["neck"], neckRot);
        currentKey->boneMatrices["lowerSpine"]=setRotationComponent(masterKey->boneMatrices["lowerSpine"], pelvisRot);
        currentKey->boneMatrices["spine"]=setRotationComponent(masterKey->boneMatrices["spine"], torsoRot);

        //switch left and right

        currentKey->boneMatrices["leftShoulder"]=setRotationComponent(masterKey->boneMatrices["leftShoulder"], rightShoulderRot);
        currentKey->boneMatrices["leftElbow"]=setRotationComponent(masterKey->boneMatrices["leftElbow"], rightElbowRot);

        currentKey->boneMatrices["rightShoulder"]=setRotationComponent(masterKey->boneMatrices["rightShoulder"], leftShoulderRot);
        currentKey->boneMatrices["rightElbow"]=setRotationComponent(masterKey->boneMatrices["rightElbow"], leftElbowRot);
        */
       // if (input->bShiftDown){
            currentKey->boneMatrices["leftHip"]=setRotationComponent(masterKey->boneMatrices["leftHip"], rightHipRot);
            currentKey->boneMatrices["leftKnee"]=setRotationComponent(masterKey->boneMatrices["leftKnee"], rightKneeRot);
            currentKey->boneMatrices["rightHip"]=setRotationComponent(masterKey->boneMatrices["rightHip"], leftHipRot);
            currentKey->boneMatrices["rightKnee"]=setRotationComponent(masterKey->boneMatrices["rightKnee"], leftKneeRot);
       // }



    /*
        for (it=masterKey->boneMatrices.begin(); it!=masterKey->boneMatrices.end(); it++)
            if (it->first=="neck")
    */
        keyFrames.push_back(currentKey);
    element=element->NextSiblingElement("skeleton");
    }


}

Matrix4f Action::setRotationComponent(Matrix4f dest, Matrix3f source){

    //source.transpose();
    dest.data[0]=source.data[0];
    dest.data[1]=source.data[1];
    dest.data[2]=source.data[2];

    dest.data[4]=source.data[3];
    dest.data[5]=source.data[4];
    dest.data[6]=source.data[5];

    dest.data[8]=source.data[6];
    dest.data[9]=source.data[7];
    dest.data[10]=source.data[8];

    return dest;
}

void Action::create(){sceneData->addAction(this);}

TiXmlElement* Action::save(TiXmlElement *root){

    TiXmlElement * element = new TiXmlElement( "Action" );
    element->SetAttribute("name", name);
    TiXmlText * text = new TiXmlText( typeid(*this).name() );
    element->LinkEndChild( text );

    std::map <std::string, memberID>::iterator it;

    //save standard properties
    for ( it=property.begin() ; it != property.end(); it++ )
      {
      TiXmlElement * property=new TiXmlElement(it->first);
      string value = memberToString(&it->second);
      property->LinkEndChild ( new TiXmlText(value));
      element->LinkEndChild( property );
      }

    //create KeyFrame Properties:
    for (uint i=0;i<keyFrames.size();i++){

        TiXmlElement * property=new TiXmlElement("KEYFRAME");
        TiXmlElement * transformKeyProperty=new TiXmlElement("TRANSFORMKEY");
        TiXmlElement * timeKeyProperty=new TiXmlElement("TIMEKEY");

            char transformValue[255];
            sprintf(transformValue,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", keyFrames[i]->transformKey.data[0],keyFrames[i]->transformKey.data[1],keyFrames[i]->transformKey.data[2],keyFrames[i]->transformKey.data[3],
                                                                                            keyFrames[i]->transformKey.data[4],keyFrames[i]->transformKey.data[5],keyFrames[i]->transformKey.data[6],keyFrames[i]->transformKey.data[7],
                                                                                            keyFrames[i]->transformKey.data[8],keyFrames[i]->transformKey.data[9],keyFrames[i]->transformKey.data[10],keyFrames[i]->transformKey.data[11],
                                                                                            keyFrames[i]->transformKey.data[12],keyFrames[i]->transformKey.data[13],keyFrames[i]->transformKey.data[14],keyFrames[i]->transformKey.data[15]);

            transformKeyProperty->LinkEndChild ( new TiXmlText( transformValue));

            property->LinkEndChild(transformKeyProperty);

            char timeValue[64];
            sprintf(timeValue,"float %f",keyFrames[i]->timeKey);
            timeKeyProperty->LinkEndChild ( new TiXmlText( timeValue));

            property->LinkEndChild(timeKeyProperty);

        if (bSkeletal){
            std::map <std::string, Matrix4f>::iterator it;
            for ( it=keyFrames[i]->boneMatrices.begin() ; it!=keyFrames[i]->boneMatrices.end() ; it++ ){
                TiXmlElement * boneProperty=new TiXmlElement("BONEMATRIX");
                boneProperty->SetAttribute("name", it->first);
                char boneValue[255];
                sprintf(boneValue,"mat4f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",  it->second.data[0],it->second.data[1],it->second.data[2],it->second.data[3],
                                                                                            it->second.data[4],it->second.data[5],it->second.data[6],it->second.data[7],
                                                                                            it->second.data[8],it->second.data[9],it->second.data[10],it->second.data[11],
                                                                                            it->second.data[12],it->second.data[13],it->second.data[14],it->second.data[15]);


                boneProperty->LinkEndChild ( new TiXmlText( boneValue));
                property->LinkEndChild(boneProperty);
                }
            }
        element->LinkEndChild( property );
        }

    return element;
}

void Action::load(TiXmlElement* myInfo){

    BasicButton::load(myInfo);

    //Location/Rotation Keys
    for ( TiXmlElement * property=myInfo->FirstChildElement("KEYFRAME") ; property!=NULL; property=property->NextSiblingElement("KEYFRAME")){
        key* myKey = new key;

        //location
        TiXmlElement * baseProperty=property->FirstChildElement("TRANSFORMKEY");
        if (baseProperty && baseProperty->GetText()!=NULL){
            myKey->transformKey=readMatrix4f((char*)baseProperty->GetText());
            //cout << "found new locationKey: " << myKey->locationKey << endl;
            }
        else
            cout << "something wrong with KeyFrame" << endl;

        //time
        TiXmlElement * timeProperty=property->FirstChildElement("TIMEKEY");
        if (timeProperty && timeProperty->GetText()!=NULL){
            myKey->timeKey=readFloat((char*)timeProperty->GetText());
            //cout << "found new timeKey: " << myKey->timeKey << endl;
            }
        else
            cout << "something wrong with KeyFrame" << endl;

        //bones
        TiXmlElement* boneProperty=property->FirstChildElement("BONEMATRIX");
        while (boneProperty){

            string boneName=boneProperty->Attribute("name");
            Matrix4f myMatrix=readMatrix4f((char*)boneProperty->GetText());
            myKey->boneMatrices[boneName]=myMatrix;
            boneProperty=boneProperty->NextSiblingElement();
            }
        keyFrames.push_back(myKey);
        //cout << "loaded KeyFrames: " << keyFrames.size() << endl;
      }

}
