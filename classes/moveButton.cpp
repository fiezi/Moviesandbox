#include "moveButton.h"
#include "input.h"


MoveButton::MoveButton(){

bActive=false;
name="LOCATION";
//fineLocation=Vector3f(0,0,0);
}

void MoveButton::update(double deltaTime){
    if (bActive && parent){
        for (int i=0;i<(int)input->selectedActors.size();i++){

            Vector3f loc;


          if (input->bShiftDown){
            fineLocation[i]+=Vector3f(0.0, input->mouseVector.y*0.1, 0.0);
            loc=fineLocation[i];
          }
          else
            {
            fineLocation[i]-=Vector3f(0.0, 1.0, 0.0).crossProduct(input->controller->orientation) *   input->mouseVector.x*0.1;
            fineLocation[i]+=Vector3f(input->controller->orientation.x, 0.0, input->controller->orientation.z) * input->mouseVector.y*0.1;
            loc=fineLocation[i];
            }
          //don't do it twice!
        input->selectedActors[i]->setLocation(loc);
    }
		  input->bConfineMouse=true;

      }

}


void MoveButton::create()
{
	renderer->addButton(this);
	cout << "creating a MoveButton!" << endl;
}
