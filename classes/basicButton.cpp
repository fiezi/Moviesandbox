#include "input.h"

BasicButton::BasicButton(){

    scale=Vector3f(30.0f,30.0f,30.0f);
    name="BasicButton";
    textureID="icon_base";
    bComputeLight=true;

    bOver=false;
    bDrawName=false;
    bDrawPrefix=true;
    bAlwaysUpdate=false;
    bPermanent=false;
    bScreenOverlay=false;
    bMessageWindow=false;
    bIndividualListSize=false;

    bDrawOrientation=false;

    bDragable=false;
    bConfineDragX=false;
    bConfineDragY=false;

    bResetAfterDrag=true;
    bTriggerWhileDragging=false;

    bPartOfMenu=false;

    tooltip="";
    tooltipOffset=Vector2f(0,0);

    parent=NULL;
    buttonProperty="NULL";

    level=0;

    sceneShaderID="buttonTexture";

    drawType=DRAW_PLANE;

    bTextured=true;
    bUseShader=true;

    registerProperties();
}

BasicButton::~BasicButton(){
}

       //setup functions
void BasicButton::registerProperties(){

    createMemberID("LEVEL",&level,this);
    createMemberID("PARENT",&parent,this);
    createMemberID("BUTTONPROPERTY",&buttonProperty,this);
    Actor::registerProperties();
}


void BasicButton::setup(){

    buttonColor=sceneData->meanButtonColor;
    mouseOverColor=sceneData->mouseOverColor;
    clickedColor=sceneData->focusButtonColor;

    tooltipOffset=Vector2f(scale.x+2,scale.y/2 +2);
    color=buttonColor;
    Actor::setup();
}

void BasicButton::update(double deltaTime){

    Actor::update(deltaTime);
    if (this==input->focusButton)
        color=clickedColor;
    else
        color=buttonColor;
}

void BasicButton::drawTooltip(){


    //tooltip rendering
    if (bOver){

        if(tooltip=="" && !bDrawName)
            renderer->drawText(name, location.x + tooltipOffset.x , location.y + tooltipOffset.y);
        else
            renderer->drawText(tooltip, location.x + tooltipOffset.x , location.y + tooltipOffset.y);
    }
    //drawname is not a tooltip! So no tooltipOffset here! instead, use drawNameOffset
    if (bDrawName){

        //get rid of prefix
        string myName=name;
        if (!bDrawPrefix){
            size_t pos=name.find(" ");
            myName = name.substr(pos);
        }
        //smallify for better readability - if we want to!
        string smallName=myName.substr(0,scale.x/8);
        if (smallName.size()<myName.size())
            smallName+="...";
        renderer->drawText((char*)smallName.c_str(), location.x+2+ drawNameOffset.x,location.y+scale.y/2+2.0 + drawNameOffset.y);
    }
    bOver=false;
}

void BasicButton::drawPlane(){

    if (bMessageWindow){
        renderer->setupShading("color");
        renderer->drawPlane(-20.0, -20.0, -20.0+400, 100, Vector4f(0.2,0.2,0.2,1.0));
    }

    renderer->drawPlane(0.0,0.0,scale.x,scale.y, color );
}

void BasicButton::mouseOver(){

    bOver=true;              // for later rendering of tooltip!
    color=mouseOverColor;
}

void BasicButton::mouseDrag(){
}

void BasicButton::finishDrag(){}

void BasicButton::clickedLeft(){
}

void BasicButton::focusClick(){
  input->focusButton=NULL;

  //destroy message window after input
  if (bMessageWindow && level>0)
    input->deselectButtons(0);
}

void BasicButton::clickedRight(){
}

void BasicButton::removeReferences(){

//    Actor::removeReferences();

    for (uint i=0;i<sceneData->buttonList.size();i++){
        if (sceneData->buttonList[i]==this)
          sceneData->buttonList.erase(sceneData->buttonList.begin()+i);
    }
    if (bPermanent){
        for (uint i=0;i<sceneData->saveableButtonList.size();i++){
            if (sceneData->saveableButtonList[i]==this)
              sceneData->saveableButtonList.erase(sceneData->saveableButtonList.begin()+i);
        }
    }

}

void BasicButton::remove(){

    removeReferences();
     delete(this);
}

void BasicButton::deselect(int depth){

if (level>depth && !bPermanent)
   remove();

}

void BasicButton::create(){

sceneData->addButton(this);
std::cout << "creating a button!";
}

TiXmlElement* BasicButton::save(TiXmlElement *root){

TiXmlElement * element = new TiXmlElement( "Button" );
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
