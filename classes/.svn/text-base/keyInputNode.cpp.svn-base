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

void KeyInputNode::create(){renderer->addNode(this);}
