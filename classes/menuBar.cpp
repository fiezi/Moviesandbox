

#include "menuBar.h"
#include "renderer.h"
#include "input.h"
#include "spriteMeshLoader.h"
MenuBar::MenuBar(){

    registerProperties();
    scale.x=64;
    scale.y=16;
    color=Vector4f(0.6,0.6,0.6,0.8);
    name="File";
}

MenuBar::~MenuBar(){}


void MenuBar::setup(){

    BasicButton::setup();
}

void MenuBar::update(double deltaTime){}

void MenuBar::mouseOver(){

    BasicButton::mouseOver();
}

void MenuBar::clickedLeft(){}
void MenuBar::clickedRight(){}

void MenuBar::focusClick(){

    BasicButton::focusClick();
}


void MenuBar::trigger(MsbObject* other){

    if (other->name=="Quit"){

        exit(0);
    }

    if (other->name=="ScaleZ 0.5"){
        sceneData->drawTool->scaleZ(0.5);
    }

    if (other->name=="ScaleZ 2.0"){
        sceneData->drawTool->scaleZ(2.0);
    }

    if (other->name=="Save Drawing As..."){

        if (sceneData->selectedActors.size()>0 ){
             SkeletalActor* skel = dynamic_cast<SkeletalActor*>(sceneData->selectedActors[0]);
            if (!skel){
                cout << "selected drawing not valid" << endl;
                return;
            }
            string filename = sceneData->saveFileDialog(".spriteMesh");

            //TODO: this only for windows! Make a function out of this!
            size_t found=filename.rfind(DIRECTORY_SEPARATION)+1;
            string smallName=filename.substr(found);
            found=smallName.rfind('.');
            smallName=smallName.substr(0,found);


            sceneData->spriteMeshLoader->saveSpriteMesh(filename, skel, smallName);

             TiXmlElement * newMesh= new TiXmlElement("SpriteMesh");
            newMesh->SetAttribute("meshID",smallName);
            newMesh->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(newMesh);
            free(newMesh);
        }else{
            cout << "no drawing selected for saving" << endl;
            return;
        }

    }

    if (other->name== "Merge Drawings"){

        sceneData->drawTool->mergeDrawings();
    }


}


void MenuBar::deselect(int depth){

    BasicButton::deselect(depth);
}

void MenuBar::create(){sceneData->addButton(this);}
