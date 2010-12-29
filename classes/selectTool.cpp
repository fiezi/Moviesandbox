

#include "selectTool.h"
#include "renderer.h"
#include "input.h"

SelectTool::SelectTool(){

bKeepSelection=false;
}

SelectTool::~SelectTool(){}

void SelectTool::setup(){

  MsbTool::setup();
}

void SelectTool::start(){

    MsbTool::start();
    myBtn->color=COLOR_RED;
    glutSetCursor(GLUT_CURSOR_INHERIT);

}

void SelectTool::stop(){

    MsbTool::stop();
    myBtn->color=COLOR_WHITE;

}

void SelectTool::keyPressed(int key){

}

void SelectTool::keyReleased(int key){

    //delete or backspace
    if (key==127 || key==8){
        if (input->worldTarget && input->worldTarget->name!="ground" && input->worldTarget->bRemoveable){
            input->worldTarget->remove();
        }
    }

    //copy selected ctrl-d
    if (key==4 && input->selectedActors.size()>0){
        copySelected();
    }

        //create Prefab
    if (key=='P'){
        input->staticButton=(BasicButton*)renderer->actorInfo["18CreatePrefabButton"].actorReference;
        input->staticButton->setLocation(Vector3f(input->screenX/2-200,input->screenY/2-50,0));
        input->staticButton->color=COLOR_BLUE;
        input->staticButton->name="name your prefab";
        input->staticButton->clickedLeft();
        renderer->buttonList.push_back(input->staticButton);
    }

    //create Group
    if (key=='G' && input->selectedActors.size()>1){            //shift-g
        makeGroup();
    }
}

void SelectTool::mousePressed(int btn){

}

void SelectTool::mouseReleased(int btn){

    MsbTool::mouseReleased(btn);


		//ACTOR CLICKING
		//must go second, because Actors could create Buttons and set them up, and we don't want them to get queried right away

        if (input->worldTarget){

			//right Button creates menu if on selected actor
			if (btn==MOUSEBTNRIGHT && input->worldTarget->name!="ground"){
			    for (int i=0;i<(int)input->selectedActors.size();i++)
                    if (input->worldTarget==input->selectedActors[i])
                        input->createActorMenu();
			}

			//any Button selects!
			if (btn==MOUSEBTNLEFT || btn==MOUSEBTNRIGHT){
				if (!bKeepSelection) selectActors(btn, input->worldTarget);
			}

		}

}

void SelectTool::mouseDragged(int btn){

}


void SelectTool::update(double deltaTime){

  MsbTool::update(deltaTime);
}

void SelectTool::selectActors(int btn, Actor* other){

    //don't do anything to selection if we're just finishing a move or rotate
    if (input->actorMenu && input->actorMenu->listButton.size()>0)
        return;

    //can select ground from layerInspector!
    if (other->name=="ground" && input->hudTarget==NULL){
        input->deselectActors();
        return;
    }

    if (input->bShiftDown){                                                                //adding to selection
        bool foundInSelection=false;
        for (int i=0;i<(int)input->selectedActors.size();i++){
            if (other==input->selectedActors[i])
                foundInSelection=true;                                              //only add if we're not already in selection
            }
        if (!foundInSelection){
            if (other->groupID!="NULL" && !input->bCtrlDown){                        //if we're part of a group, add the whole group
                for (int i=0;i<(int)renderer->actorList.size();i++){                //unless we hold down ctrl, then only add this one!
                    if (renderer->actorList[i]->groupID==other->groupID){
                        renderer->actorList[i]->bSelected=true;
                        input->selectedActors.push_back(renderer->actorList[i]);
                        }
                    }
                }
            else{
                other->bSelected=true;
                input->selectedActors.push_back(other);                              //no CTRL or no group, but SHIFT down, so should add
                }
            }
        }
    else{
        bool foundInSelection=false;
        for (int i=0;i<(int)input->selectedActors.size();i++){
            if (other==input->selectedActors[i])
                foundInSelection=true;                                              //check if we're not already in selection
            }
        if (!foundInSelection && btn==MOUSEBTNLEFT){                                                     //if we're clicking on a actor not already in selection, you can go ahead
            input->deselectActors();
            if (other->groupID!="NULL" && !input->bCtrlDown){                            //if we're part of a group, select the whole group
                for (int i=0;i<(int)renderer->actorList.size();i++){                    //unless we hold down CTRL, then only select me
                    if (renderer->actorList[i]->groupID==other->groupID){
                        renderer->actorList[i]->bSelected=true;
                        input->selectedActors.push_back(renderer->actorList[i]);
                        }
                    }
                }
            else{
                other->bSelected=true;
                input->selectedActors.push_back(other);                                  //no CTRL or no Group
                }
            }
        else if (!foundInSelection && btn==MOUSEBTNRIGHT){                           //not in selection and clicked right
                other->bSelected=true;
                input->selectedActors.push_back(other);
                }
        }
}

void SelectTool::makeGroup(){

        TextInputButton* messageWindow=new TextInputButton;
        messageWindow->name="GroupName:";
        messageWindow->bMessageWindow=true;
        messageWindow->parent=input->selectedActors[0];
        messageWindow->buttonProperty="GROUPID";
        messageWindow->setLocation(Vector3f(input->screenX/2-200,input->screenY/2-50,0));
        messageWindow->scale=Vector3f(100,50,1);
        messageWindow->sceneShaderID="color";
        input->focusButton=messageWindow;
        messageWindow->level=100;
        messageWindow->setup();
        messageWindow->bDrawName=true;
        messageWindow->color=COLOR_GREEN;
        renderer->buttonList.push_back(messageWindow);
        messageWindow->clickedLeft();
}


void SelectTool::makePrefab(std::string prefabName){

        //assemble TiXmlElement of all selected Actors
        TiXmlElement* myPrefab = new TiXmlElement("MovieSandbox");
        //save in order of actorList for later referencing!

        //we have all prefabActors in a list - the selectActors list!
        //where's the problem? Serialising the base stuff! How can we make sure that we only save the relative bases?
        //write it in the prefab file! Maybe...


        //finally, save
        for (int i=0;i<(int)input->selectedActors.size();i++){
            myPrefab->LinkEndChild(input->selectedActors[i]->saveAsPrefab(myPrefab));
        }

        //save to disk
        TiXmlDocument doc;
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        doc.LinkEndChild( decl );
        doc.LinkEndChild(myPrefab);
        string stringName="resources/prefabs/"+prefabName+".prefab";
        doc.SaveFile(stringName);

        for (int i=0;i<(int)input->prefabs.size();i++){
            if (input->prefabs[i]==prefabName+".prefab");
                return;
        }
        input->prefabs.push_back(prefabName+".prefab");

}

void SelectTool::copySelected(){

    cout << "copying..." << endl;
    for (int i=0;i<(int)input->selectedActors.size();i++){
        const std::type_info* myType=&(typeid(*(input->selectedActors[i])));
        Actor * A=renderer->actorInfo[myType->name()].actorReference;
        A->create();

        TiXmlElement * root = new TiXmlElement( "Moviesandbox" );
        A=renderer->actorList[renderer->actorList.size()-1];
        A->load(input->selectedActors[i]->save(root));
        delete root;
        A->bSelected=false;                                             //flip selection when copying!
        A->setup();
        }
}

