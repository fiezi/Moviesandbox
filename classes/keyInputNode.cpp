#include "keyInputNode.h"
#include "control.h"
#include "input.h"

KeyInputNode::KeyInputNode(){
    name="KeyInputRoot";
    keyInput=" ";
    bKeyInputBranch=true;
    treeStart=this;
    listType.push_back("15TextInputButton");
    listProp.push_back("KEYINPUT");
    listIcon.push_back("icon_flat");


    buttonColor=Vector4f(0.5,1.0,0.5,1.0);
    mouseOverColor=Vector4f(0.7,1.0,0.7,1.0);

    registerProperties();
}

KeyInputNode::~KeyInputNode(){}

void KeyInputNode::registerProperties(){

Node::registerProperties();
createMemberID("KEYINPUT",&keyInput,this);
}

void KeyInputNode::execute(){

    if (input->lastKey==*keyInput.c_str()){

        cout << "our keyInput string is: " << keyInput << endl;
        cout << "our nodeTree is: " << nodeBranch << endl;
        //cout << "yay! It works" << endl;
        Node::nextNode();
        }

}

void KeyInputNode::create(){sceneData->addNode(this);}
