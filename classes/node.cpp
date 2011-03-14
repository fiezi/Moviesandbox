#include "node.h"
#include "input.h"

Node::Node(){

    name="Node";
    bKeyInputBranch=false;
    bPermanent=true;
    bDragable=true;
    childNode=NULL;
    nodeIn=NULL;
    nodeOut=NULL;
    treeStart=NULL;
    bDrawName=true;
    bDrawListNames=true;
    listWidth=100;
    listHeight=12;
    listColor=Vector4f(0.8,0.0,0.2,1.0);


    nodeBranch=0;

    scale.x=74;
    scale.y=16;

    textureID="icon_flat";

//this is a bit hacky, as it creates NodeIO for all the "sleeping" nodes for our actor references...
//but is needed so we can connect nodes when loading...

    nodeIn= new NodeIO;
    nodeOut = new NodeIO;

    nodeIn->parent=this;
    nodeOut->parent=this;

    registerProperties();
}

Node::~Node(){

}


void Node::registerProperties(){
    BasicButton::registerProperties();
    createMemberID("CHILDNODE",&childNode,this);
    createMemberID("BKEYINPUTBRANCH",&bKeyInputBranch,this);
    createMemberID("TREESTART",&treeStart,this);
}

void Node::setup(){

    renderer->buttonList.push_back(nodeIn);
    renderer->buttonList.push_back(nodeOut);

    connectChild(childNode);
}

void Node::update(double deltaTime){

ListButton::update(deltaTime);

if (nodeIn){
  nodeIn->setLocation(location - Vector3f( 0, nodeIn->scale.y, 0));
  }
if (nodeOut){
  nodeOut->setLocation(location + Vector3f(scale.x-nodeOut->scale.x, scale.y, 0 ) );
  }
}



void Node::connectChild(Node* connectNode){

    //connect childNodes!
    childNode=connectNode;

    if (childNode){
        cout << "we have a child loaded!" << endl;
        if (childNode->nodeIn){
            nodeOut->connector=childNode->nodeIn;
            nodeOut->bConnected=true;
            childNode->nodeIn->bConnected=true;
            }
        }

}

void Node::mouseOver(){

    //ListButton::mouseOver();
    //TODO: highlight dependent Actors
}

void Node::mouseDrag(){

input->dragButton=this;
location.x=input->mouseX-scale.x/2;
location.y=input->mouseY-scale.y/2;
}

void Node::clickedLeft(){

//ListButton::clickedLeft();
}

void Node::clickedRight(){

ListButton::clickedLeft();
}


void Node::trigger(MsbObject* other){

//callback function for Propertybuttons
}


void Node::start(){

bHidden=true;
if (nodeIn)
  nodeIn->bHidden=true;
if (nodeOut)
  nodeOut->bHidden=true;
}

void Node::stop(){

bHidden=false;
if (nodeIn)
  nodeIn->bHidden=false;
if (nodeOut)
  nodeOut->bHidden=false;
}

void Node::execute(){

cout << "executing node in tree "<< nodeBranch << endl;
nextNode();
}


void Node::nextNode(){

    cout << "next node in tree "<< nodeBranch << " tree size is: " << Control::nodeTree.size() <<endl;

    if (childNode)
      {
      childNode->nodeBranch=nodeBranch;
      Control::nodeTree[nodeBranch]=childNode;
      }
    else if (bKeyInputBranch)
      resetBranch();
    else
      removeBranch();
}

void Node::resetBranch(){
    //go back up to keyInputNode!
      Control::nodeTree[nodeBranch]=treeStart;
      cout << "resetting branch!" << endl;
    }

void Node::removeBranch(){

for (unsigned int i=nodeBranch+1;i<Control::nodeTree.size();i++){   //adjust nodeTree
  if (Control::nodeTree[i])
    Control::nodeTree[i]->nodeBranch--;
}
Control::nodeTree.erase(Control::nodeTree.begin()+nodeBranch);    //delete from NodeTree

std::cout << "Script Branch ended!" << endl;
}

void Node::remove(){

    //remove references in nodes
    //go through all nodes
    for (unsigned int i=0;i<renderer->nodeList.size();i++){
        Node* myNode=renderer->nodeList[i];
        //go through all properties
        std::map <std::string, memberID>::iterator it;
        for ( it=myNode->property.begin() ; it != myNode->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(Node*));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                Node** pn=(Node**)mID.memberReference;
                //see if it references the Actor we want to delete!
                if (*pn==this){
                    *pn=NULL;   //and set the reference to NULL!
                    }
                }
            }
        }
    //remove references in Actors
    //go through all Actors
    for (unsigned int i=0;i<renderer->actorList.size();i++){
        Actor* myActor=renderer->actorList[i];
        //go through all properties
        std::map <std::string, memberID>::iterator it;
        for ( it=myActor->property.begin() ; it != myActor->property.end(); it++ ){
            //find Actor* properties
            memberID mID=it->second;
            const std::type_info* mType;
            mType=&(typeid(Node*));
            if (mID.memberType->name()==mType->name()){
                //found an actor property!
                Node** pn=(Node**)mID.memberReference;
                //see if it references the Actor we want to delete!
                if (*pn==this){
                    *pn=NULL;   //and set the reference to NULL!
                    }
                }
            }
        }


    for (uint i=0;i<renderer->buttonList.size();i++){
        if (renderer->buttonList[i]==this)
          renderer->buttonList.erase(renderer->buttonList.begin()+i);
    }

    for (uint i=0;i<renderer->nodeList.size();i++){
        if (renderer->nodeList[i]==this)
          renderer->nodeList.erase(renderer->nodeList.begin()+i);
    }

    if (nodeIn)
        nodeIn->remove();
    if (nodeOut)
        nodeOut->remove();

    delete(this);
}

void Node::create(){

renderer->addNode(this);
std::cout << "creating a Node!";
}


TiXmlElement* Node::save(TiXmlElement *root){

    TiXmlElement * element = new TiXmlElement( "Node" );
    element->SetAttribute("name", name);
    TiXmlText * text = new TiXmlText( typeid(*this).name() );
    element->LinkEndChild( text );

    std::map <std::string, memberID>::iterator it;

    for ( it=property.begin() ; it != property.end(); it++ )
      {
      TiXmlElement * property=new TiXmlElement(it->first);
      string value = memberToString(&it->second);
      property->LinkEndChild ( new TiXmlText( value));
      element->LinkEndChild( property );
      }

    return element;
}
