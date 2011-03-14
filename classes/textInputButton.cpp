

#include "textInputButton.h"
#include "renderer.h"
#include "input.h"
#include "inputConnectButton.h"

TextInputButton::TextInputButton(){

name="TextInputButton";
parent=NULL;
buttonProperty="NULL";
bEditing=false;
}

TextInputButton::~TextInputButton(){}


void TextInputButton::setup(){
BasicButton::setup();

if (parent && buttonProperty!="NULL"){
    memberID * mID=&parent->property[buttonProperty];
    if (mID)
        tooltip=memberToString(mID);
    }
}

void TextInputButton::update(double deltaTime){


    if (bEditing){
        tooltip=input->inputText;
        if ((int)(renderer->currentTime)%1000 < 500)
            tooltip+="|";
    }
}

void TextInputButton::drawTooltip(){

    bOver=true;

    BasicButton::drawTooltip();
}

void TextInputButton::mouseOver(){

BasicButton::mouseOver();
}

void TextInputButton::mouseDrag(){}

void TextInputButton::clickedLeft(){

bEditing=true;
input->focusButton=this;
input->inputText="";
input->bTextInput=true;
//but also: read the type of the property, if we're connected to one and set it to the inputString!
if (parent && buttonProperty!="" && tooltip!=""){
    //go through tooltip and stop when we find a " "!
    char* myTooltip=(char*)tooltip.c_str();
    char* preText = strtok(myTooltip, " "); // Splits spaces between words in st
    cout << "our pretext would be: " << preText << endl;
    input->inputText=preText;
    input->inputText+=" ";
    }
}

//use this to connect properties to InputConnect Button!
void TextInputButton::clickedRight(){

    input->focusButton=this;

    cout << "connecting..." << input->hudTarget << endl;

}

void TextInputButton::focusClick(){

    InputConnectButton* ipcb =NULL;
    ipcb=dynamic_cast<InputConnectButton*>(input->hudTarget);

    //connecting with InputConnectButton
    if (input->hudTarget && ipcb){

        cout << "connecting with: " << input->hudTarget << endl;
        input->hudTarget->buttonProperty=buttonProperty;                //which property do we want to connect?
        input->hudTarget->trigger(parent);                          //which actor do we want to connect

    }else{

        std::stringstream ss;
        tooltip=(char*)input->inputText.c_str();

        //adding selectedActor stack to TextInputButton

        if (parent && buttonProperty!="NULL"){

            bool bParentSelected=false;
            for (int i=0;i<(int)input->selectedActors.size();i++ )
                if (input->selectedActors[i]==parent)
                    bParentSelected=true;

           if (bParentSelected){

                for (int i=0;i<(int)input->selectedActors.size();i++){

                    memberID * mID = &(input->selectedActors[i]->property[buttonProperty]);        //look for the property we should set
                    cout << "we found the following memberID: " << mID->memberName << " with the following member type: " << mID->memberType->name() << endl;
                    if (buttonProperty=="ROTATION"){
                        Vector3f rot = readVector3f((char*)input->inputText.c_str());
                        input->selectedActors[i]->setRotation(rot);
                    }else if (buttonProperty=="LOCATION"){
                        Vector3f loc = readVector3f((char*)input->inputText.c_str());
                        input->selectedActors[i]->setLocation(loc);
                    }else if (buttonProperty=="SCALE"){
                        Vector3f loc = readVector3f((char*)input->inputText.c_str());
                        input->selectedActors[i]->setScale(loc);
                    }else if (mID){
                        memberFromString(mID,input->inputText);
                    }else
                        cout << "no such property: " << buttonProperty;
                }
           }
           else{
               cout << "we are connected to " << parent->name <<endl;

               memberID * mID = &(parent->property[buttonProperty]);        //look for the property we should set
               cout << "we found the following memberID: " << mID->memberName << " with the following member type: " << mID->memberType->name() << endl;
               if (mID)
                 {
                 memberFromString(mID,input->inputText);
                 }
               else
                 cout << "no such property: " << buttonProperty;
               }
          }

        if (parent)
          parent->trigger(this);

        //clean up

        input=Input::getInstance();

        input->inputText="NULL";

        bEditing=false;
        }
    BasicButton::focusClick();
}

void TextInputButton::deselect(int depth){

BasicButton::deselect(depth);
}

void TextInputButton::create(){sceneData->addButton(this);}
