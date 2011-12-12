#include "moveButton.h"
#include "input.h"


MoveButton::MoveButton(){

bActive=false;
bDragable=true;
bResetAfterDrag=true;
name="LOCATION";
//fineLocation=Vector3f(0,0,0);
}

void MoveButton::update(double deltaTime){

    BasicButton::update(deltaTime);

    if (bActive && parent){
        buttonColor.a=0.15;

        for (int i=0;i<(int)sceneData->selectedActors.size();i++){


        if (fineLocation.size()<=i)
            return;

         Vector3f loc;

          if (input->bShiftDown){
            fineLocation[i]+=Vector3f(0.0, input->mouseVector.y*0.1, 0.0);
            loc=fineLocation[i];
          }
          else
            {
            fineLocation[i]-=Vector3f(0.0, 1.0, 0.0).crossProduct(sceneData->controller->orientation) *   input->mouseVector.x*0.1;
            fineLocation[i]+=Vector3f(sceneData->controller->orientation.x, 0.0, sceneData->controller->orientation.z) * input->mouseVector.y*0.1;
            loc=fineLocation[i];
            }
          //don't do it twice!
        sceneData->selectedActors[i]->setLocation(loc);
        }

    input->bConfineMouse=true;
    }

}


void MoveButton::create()
{
	sceneData->addButton(this);
	cout << "creating a MoveButton!" << endl;
}
