

#include "layerInspector.h"
#include "renderer.h"
#include "input.h"
LayerInspector::LayerInspector(){
    level=0;
    listColumns=1;
    newLayer=NULL;
    listHeight=12;
    listWidth=120;
    maxListItems=40;
    registerProperties();
}

LayerInspector::~LayerInspector(){}

void LayerInspector::registerProperties(){

    Inspector::registerProperties();
    createMemberID("ACTORREFERENCES",&actorReferences, this);
}

void LayerInspector::createInspectorButtons(){

    newLayer= new AssignButton;
    newLayer->parent=this;
    renderer->buttonList.push_back(newLayer);
    newLayer->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    newLayer->name="new Layer";
    newLayer->color=Vector4f(1,0,0,1);
    inspectorButtons.push_back(newLayer);
}

void LayerInspector::refreshList(){

    int laySize=renderer->layerList.size();
    int actSize=renderer->actorList.size();
    int listSize=listButton.size();

    if ( listSize !=laySize + actSize ){
        assembleList();
        //open
        clickedLeft();
        //close
        clickedLeft();
        //or vice versa
    }

    //refresh Actor names
    for (int i=0;i<(int)actorButtons.size();i++){
        if ((int)actorReferences.size()>i && actorReferences[i])
            actorButtons[i]->name=actorReferences[i]->name;
    }
}

void LayerInspector::assembleList(){

    //remove all buttons

    for (int i=0;i<(int)actorButtons.size();i++){
        actorButtons[i]->remove();
    }

    for (int i=0;i<(int)layerButtons.size();i++){
        layerButtons[i]->remove();
    }

    if (scrollBar){
        scrollBar->remove();
        scrollBar=NULL;
    }
    listButton.clear();
    actorButtons.clear();
    layerButtons.clear();
    actorReferences.clear();

    for (int i=0;i<(int)renderer->layerList.size();i++){
        AssignButton* l= new AssignButton;
        renderer->buttonList.push_back(l);
        layerButtons.push_back(l);

        l->parent=this;
        l->name=renderer->layerList[i]->name;
        l->level=level+1;
        l->bDrawName=true;
        l->color=Vector4f(0.8,0.6,0.6,1.0);

		//actions are just set to be visible, not actually created!
        l->bHidden=true;
        l->bDragable=true;
        l->bPermanent=true;

        l->scale=Vector3f(40,30,1);
        l->sceneShaderID="color";

        if (listWidth>0)
            l->scale.x=listWidth;
        if (listHeight>0)
            l->scale.y=listHeight;

        //l->textureID=renderer->layerList[i]->textureID;
        listButton.push_back(l);
        placeButton(listButton.size()-1,listButton.size()-1);

        for (int j=0;j<(int)renderer->layerList[i]->actorList.size();j++){

            AssignButton* a= new AssignButton;
            renderer->buttonList.push_back(a);
            actorButtons.push_back(a);
            actorReferences.push_back(renderer->layerList[i]->actorList[j]);

            a->parent=this;

            a->name=renderer->layerList[i]->actorList[j]->name;
            a->color=Vector4f(0.4,0.4,0.4,1.0);

            a->level=level+1;
            a->bDrawName=true;
            a->bHidden=true;
            a->bPermanent=true;

            a->scale=Vector3f(tabWidth, 30.0f, 1.0f);
            a->sceneShaderID="color";
            a->color=Vector4f(0.6,0.6,0.6,1);
            listButton.push_back(a);
            placeButton(listButton.size()-1,listButton.size()-1);
        }
    }
}

void LayerInspector::trigger(Actor* other){
    Inspector::trigger(other);

    if (other==newLayer){
        renderer->addLayer("newLayer"+layerButtons.size());
    }
    for (int i=0;i<(int)actorButtons.size();i++){
        if (other==actorButtons[i]){
            input->worldTarget=actorReferences[i];

            input->controller->currentTool->selectActors(MOUSEBTNLEFT,input->worldTarget);
        }
    }
}
void LayerInspector::create(){renderer->addButton(this);}
