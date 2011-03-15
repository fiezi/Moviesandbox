#include "input.h"

BasicButton::BasicButton(){

scale=Vector3f(30.0f,30.0f,30.0f);
name="BasicButton";
textureID="icon_base";
bComputeLight=false;

bOver=false;
bDrawName=false;
bAlwaysUpdate=false;
bPermanent=false;
bScreenOverlay=false;
bMessageWindow=false;
bDrawOrientation=false;

bDragable=false;
bConfineDragX=false;
bConfineDragY=false;

bResetAfterDrag=true;
bTriggerWhileDragging=false;

tooltip="";
tooltipOffset=Vector2f(0,0);

parent=NULL;
buttonProperty="NULL";

level=0;

sceneShaderID="texture";

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

tooltipOffset=Vector2f(scale.x+2,scale.y/2 +2);

Actor::setup();
}

void BasicButton::update(double deltaTime){

Actor::update(deltaTime);
}

void BasicButton::drawTooltip(){

//tooltip rendering
if (bOver)
  {
  if(tooltip=="" && !bDrawName)
    renderer->drawText(name, location.x + tooltipOffset.x , location.y + tooltipOffset.y);
  else
    renderer->drawText(tooltip, location.x + tooltipOffset.x , location.y + tooltipOffset.y);
  }
//drawname is not a tooltip! So no tooltipOffset here!
if (bDrawName)
    renderer->drawText((char*)name.c_str(), location.x+2,location.y+scale.y/2+2.0);
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

void BasicButton::remove(){

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
