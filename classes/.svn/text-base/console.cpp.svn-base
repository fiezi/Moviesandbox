

#include "console.h"
#include "renderer.h"
#include "input.h"
#include "physicsActor.h"


Console::Console(){

registerProperties();
bDrawName=true;
toCall=NULL;
level=0;
name="Console";
//tooltip="Enter Command";
scale.x= 50;
scale.y= 10;
color=Vector4f(0.7,0.7,0.7,1.0);
}


Console::~Console(){}

void Console::registerProperties(){

TextInputButton::registerProperties();

functionList["randomSize"]=&Console::randomSize;
functionList["dropBird"]=&Console::dropBird;
functionList["setZero"]=&Console::setZero;
}

void Console::focusClick(){

    std::string funcPart, argPart;

    size_t pos;

    pos = input->inputText.find(" ");    // position of " " in str
    funcPart = input->inputText.substr (0,pos); // function name
    argPart = input->inputText.substr (pos+1);   // arguments



    if (functionList[funcPart]){
        toCall=functionList[funcPart];
        (this->*toCall)(argPart);
    }

TextInputButton::focusClick();
tooltip="";
}

void Console::callFunction(string command){


    std::string funcPart, argPart;

    size_t pos;

    pos = command.find(" ");    // position of " " in str
    funcPart = command.substr (0,pos); // function name
    argPart = command.substr (pos+1);   // arguments



    if (functionList[funcPart]){
        toCall=functionList[funcPart];
        (this->*toCall)(argPart);
    }

tooltip="";
}

void Console::create(){renderer->addButton(this);}

void Console::randomSize(string args){

cout << "yay!" << args << endl;
}

void Console::dropBird(string args){

//moo
}

void Console::setZero(string args){


    cout << "setting to zero..." << endl;
    input->controller->controlledActor->setRotation(Vector3f(0,0,0));
//    input->controller->navBtn->processMove(0);
}
