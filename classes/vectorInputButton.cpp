

#include "vectorInputButton.h"
#include "renderer.h"
#include "input.h"
VectorInputButton::VectorInputButton(){

    scale.x=150-32;
    scale.y=64;
    //scale.y=49;
    listHeight=15;
    listWidth=32;
    listColor=Vector4f(0.5,0.5,0.5,1.0);
    bIndividualListSize=true;
    registerProperties();
    vecLength=3;
}


VectorInputButton::~VectorInputButton(){}


void VectorInputButton::registerProperties(){

}

void VectorInputButton::setup(){
    ListButton::setup();

    listType.push_back("15TextInputButton");
    listName.push_back("X:");
    listParent.push_back("PARENT");
    listProp.push_back(buttonProperty);

    listType.push_back("15TextInputButton");
    listName.push_back("Y:");
    listParent.push_back("PARENT");
    listProp.push_back(buttonProperty);

    listType.push_back("15TextInputButton");
    listName.push_back("Z:");
    listParent.push_back("PARENT");
    listProp.push_back(buttonProperty);

    if (parent->property[buttonProperty].memberType->name()==typeid(Vector4f).name()){

        listType.push_back("15TextInputButton");
        listName.push_back("W:");
        listParent.push_back("PARENT");
        listProp.push_back(buttonProperty);
        vecLength=4;
    }


    assembleList();
}

void VectorInputButton::update(double deltaTime){

    ListButton::update(deltaTime);

    if (listButton.size()==0)
        return;

    for (int i=0;i<vecLength;i++){
        listButton[i]->location.x=location.x+scale.x;
        listButton[i]->location.y=location.y+i*(listHeight+2.0);
        listButton[i]->setLocation(listButton[i]->location);
        listButton[i]->bHidden=bHidden;
    }
}

void VectorInputButton::mouseOver(){

BasicButton::mouseOver();
}

void VectorInputButton::mouseDrag(){}
void VectorInputButton::finishDrag(){}

void VectorInputButton::clickedLeft(){
    //ListButton::clickedLeft();
}

void VectorInputButton::clickedRight(){}

void VectorInputButton::focusClick(){

    //ListButton::focusClick();
}


void VectorInputButton::assembleList(){

    ListButton::assembleList();
    for (int i=0;i<vecLength;i++){
       ((TextInputButton*)listButton[i])->bmIDPart=true;
       ((TextInputButton*)listButton[i])->mIDPartNumber=i;
       listButton[i]->sceneShaderID="color";
        listButton[i]->bPermanent=bPermanent;
       listButton[i]->setup();
    }
}

void VectorInputButton::remove(){

    for (int i=0;i<(int)listButton.size();i++){
        listButton[i]->bPermanent=false;
        listButton[i]->level=100;
    }

	for (int i=listButton.size()-1;i>=0;i--){
		listButton[i]->remove();
	}
    listButton.clear();

    ListButton::remove();

}

void VectorInputButton::create(){sceneData->addButton(this);}
