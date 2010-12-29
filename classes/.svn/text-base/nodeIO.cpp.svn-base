#include "nodeIO.h"
#include "input.h"

NodeIO::NodeIO(){

    color=Vector4f(0,0,0,1);
    minScale=Vector3f(12,12,1);
    scale=minScale;
    maxScale=Vector3f(16,16,1);

    bDrawLine=false;
    bConnected=false;
    connector=NULL;
    name="nodeIO";
    tooltip="connect!";
}

NodeIO::~NodeIO(){
}

void NodeIO::setup(){


}

void NodeIO::update(double deltaTime){

}

void NodeIO::mouseOver(){

BasicButton::mouseOver();

if (scale.length()<maxScale.length())
  scale*=1.1;
}

void NodeIO::drawTooltip(){


    if (!bOver && scale.length()>minScale.length())
        scale*=0.9;

    BasicButton::drawTooltip();

    if (bDrawLine){
        renderer->setupShading("color");
        renderer->drawLine( Vector3f(location.x,location.y,location.z),
                            Vector3f(input->mouseX,input->mouseY,0.0),
                            Vector4f(1,0,0,1),Vector4f(1,0,0,1));
    }
    else if (bConnected && ((Node*)parent)->nodeOut==this){

      renderer->setupShading("color");
      Vector3f distance=connector->location-location;

        renderer->drawLine( Vector3f(scale.x/2 + location.x,scale.y/2+location.y,0),
                            Vector3f(scale.x/2 + location.x+distance.x/2,scale.y/2+location.y,0),
                            Vector4f(1,0,0,1),Vector4f(1,0,0,1));

        renderer->drawLine( Vector3f(scale.x/2 + location.x+distance.x/2,scale.y/2+location.y,0),
                            Vector3f(scale.x/2 + location.x+distance.x/2,connector->scale.y/2+connector->location.y,0),
                            Vector4f(1,0,0,1),Vector4f(1,0,0,1));

        renderer->drawLine( Vector3f(scale.x/2 + location.x+distance.x/2,connector->scale.y/2+connector->location.y,0),
                            Vector3f(connector->scale.x/2 + connector->location.x,connector->scale.y/2+connector->location.y,0),
                            Vector4f(1,0,0,1),Vector4f(1,0,0,1));

      renderer->setupShading("font");
    }


}

void NodeIO::drawPlane(){

BasicButton::drawPlane();

}

void NodeIO::clickedLeft(){

    Node * myNode;
    Node * connectNode;

    input->focusButton=this;

    //disconnect
    if (connector)                                      //if we were connected
      {
      connector->bConnected=false;                      //disable line drawing from connector's side
      connector->connector=NULL;                        //detach from connector
      connectNode=(Node *)connector->parent;
      myNode=(Node *) parent;
      if (connector==connectNode->nodeOut)              //if we were connected to a NodeOut (which means we must be a NodeIn!)
        connectNode->childNode=NULL;                    //remove childNode from connected Node
      else                                              //we are connected to a NodeIn (which means we are a NodeOut!)
        myNode->childNode=NULL;                         //remove childNode from our Node
      }
    connector=NULL;
    bDrawLine=true;
    bConnected=false;
}

void NodeIO::focusClick(){

Node * myNode;
Node * connectNode;

BasicButton::focusClick();



connector=dynamic_cast<NodeIO *>(input->hudTarget);

if (connector)
  {
  connectNode=(Node *)connector->parent;
  myNode=(Node *)parent;
  //Out to in!
  if (myNode->nodeOut==this && connectNode->nodeIn==connector)
    {
    bConnected=true;
    myNode->childNode=connectNode;
    connectNode->treeStart=myNode->treeStart;

    if (myNode->bKeyInputBranch)
        connectNode->bKeyInputBranch=true;
    else
        connectNode->bKeyInputBranch=false;

    connector->connector=this;              //link back
    connector->bConnected=true;
    }
  //In to Out!
  if (myNode->nodeIn==this && connectNode->nodeOut==connector)
    {
    bConnected=true;
    connectNode->childNode=myNode;
    myNode->treeStart=connectNode->treeStart;

    if (connectNode->bKeyInputBranch)
        myNode->bKeyInputBranch=true;
    else
        myNode->bKeyInputBranch=false;

    connector->connector=this;      //link back
    connector->bConnected=true;
    }
  }
bDrawLine=false;
}

void NodeIO::remove(){

    if(connector){
      connector->bConnected=false;
      connector->connector=NULL;
    }

    for (uint i=0;i<renderer->buttonList.size();i++){
        if (renderer->buttonList[i]==this)
          renderer->buttonList.erase(renderer->buttonList.begin()+i);
    }
    delete(this);
}
