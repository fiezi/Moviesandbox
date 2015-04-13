

#include "selectTool.h"
#include "renderer.h"
#include "input.h"

SelectTool::SelectTool(){

bKeepSelection=false;
bClickedonGizmo=false;

clipboard=NULL;
}

SelectTool::~SelectTool(){}

void SelectTool::setup(){

  MsbTool::setup();
}

void SelectTool::start(){

    MsbTool::start();
    myBtn->color=COLOR_RED;

    if (!sceneData->controller->bRunning)
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
        if (input->hudTarget){
                Node* n=dynamic_cast<Node*>(input->hudTarget);
                if (n)  {
                    input->deselectButtons(0);
                    n->remove();
                    return;
                }
        }
        if (input->worldTarget && input->worldTarget->name!="ground" && input->worldTarget->bRemoveable){
            input->worldTarget->remove();
        }
    }

    //copy selected ctrl-d
    if (key==4 && sceneData->selectedActors.size()>0){
        duplicateSelected();
    }


	//TODO: make MAC OS X compatible!

    //cut selected
    if (key==24&& sceneData->selectedActors.size()>0){
        cutSelected();
    }
    //copy selected
    if (key==3&& sceneData->selectedActors.size()>0){
        copySelected();
    }

    //paste selected
    if (key==22){
            pasteSelected();
    }

        //create Prefab
    if (key=='P'){
        sceneData->staticButton=(BasicButton*)sceneData->actorInfo["18CreatePrefabButton"].actorReference;
        sceneData->staticButton->setLocation(Vector3f(renderer->windowX/2-200,renderer->windowY/2-50,0));
        sceneData->staticButton->color=COLOR_BLUE;
        sceneData->staticButton->name="name your prefab";
        sceneData->staticButton->clickedLeft();
        sceneData->buttonList.push_back(sceneData->staticButton);
    }

    //create Group
    if (key=='G' && sceneData->selectedActors.size()>1){            //shift-g
        makeGroup();
    }

    if (key==220){//CTRL-whatever that thing is next to 1
        input->bQuadWarp=!input->bQuadWarp;
    }
    if (key==49){//CTRL-1
        input->qwCurrentPoint=0;
    }
    if (key==50){//CTRL-2
        input->qwCurrentPoint=1;
    }
    if (key==51){//CTRL-3
        input->qwCurrentPoint=2;
    }
    if (key==52){//CTRL-4
        input->qwCurrentPoint=3;
    }

}

void SelectTool::mousePressed(int btn){

    if (btn==MOUSEBTNLEFT && !sceneData->actorMenu && !input->hudTarget){

        if (input->worldTarget==sceneData->aGizmo->xAxisGizmo){
            bClickedonGizmo=true;
            sceneData->aGizmo->moveAxis=&(sceneData->selectedActors[0]->xAxis);
            sceneData->aGizmo->beginGizmoing();
        }

        if (input->worldTarget==sceneData->aGizmo->yAxisGizmo){
            bClickedonGizmo=true;
            sceneData->aGizmo->moveAxis=&(sceneData->selectedActors[0]->yAxis);
            sceneData->aGizmo->beginGizmoing();
        }

        if (input->worldTarget==sceneData->aGizmo->zAxisGizmo){
            bClickedonGizmo=true;
            sceneData->aGizmo->moveAxis=&(sceneData->selectedActors[0]->zAxis);
            sceneData->aGizmo->beginGizmoing();
        }

        if (input->worldTarget==sceneData->aGizmo->xRotateGizmo){
            bClickedonGizmo=true;
            sceneData->aGizmo->rotAxis=&(sceneData->selectedActors[0]->zAxis);
            sceneData->aGizmo->beginGizmoing();
        }

        if (input->worldTarget==sceneData->aGizmo->yRotateGizmo){
            bClickedonGizmo=true;
            sceneData->aGizmo->rotAxis=&(sceneData->selectedActors[0]->xAxis);
            sceneData->aGizmo->beginGizmoing();
        }

        if (input->worldTarget==sceneData->aGizmo->zRotateGizmo){
            bClickedonGizmo=true;
            sceneData->aGizmo->rotAxis=&(sceneData->selectedActors[0]->yAxis);
            sceneData->aGizmo->beginGizmoing();
        }
    }
}

void SelectTool::mouseReleased(int btn){

    MsbTool::mouseReleased(btn);


		//ACTOR CLICKING
		//must go second, because Actors could create Buttons and set them up, and we don't want them to get queried right away

        if (input->worldTarget){

			//right Button creates menu if on selected actor
			if (btn==MOUSEBTNRIGHT && input->worldTarget->name!="ground"){
			    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
                    if (input->worldTarget==sceneData->selectedActors[i]){
                        sceneData->createActorMenu();
                        return;
                    }
			    }
			    //also create menu when right-clicking on gizmos
			    if (input->worldTarget == sceneData->aGizmo->xAxisGizmo ||
                    input->worldTarget == sceneData->aGizmo->yAxisGizmo ||
                    input->worldTarget == sceneData->aGizmo->zAxisGizmo ||

                    input->worldTarget == sceneData->aGizmo->xRotateGizmo ||
                    input->worldTarget == sceneData->aGizmo->yRotateGizmo ||
                    input->worldTarget == sceneData->aGizmo->zRotateGizmo
                    ){
                        //trickery! worldTarget should be our actor to spawn menu for, not the gizmo!
                        input->worldTarget=sceneData->selectedActors[0];
                        sceneData->createActorMenu();
                        return;
                    }
			}

			//any Button selects!
			if (btn==MOUSEBTNLEFT || btn==MOUSEBTNRIGHT){
				if (!bKeepSelection && !bClickedonGizmo ) selectActors(btn, input->worldTarget);
			}

		}

    if (bClickedonGizmo){
        bClickedonGizmo=false;
        sceneData->aGizmo->endGizmoing();
    }
}

void SelectTool::mouseDragged(int btn){

}


void SelectTool::update(double deltaTime){

  MsbTool::update(deltaTime);


}

void SelectTool::selectActors(int btn, Actor* other){

    if (other)
        cout << "to be selected name:  " << other->name << " ***************************************************" << endl;
    else{
        cout << "object doesn't exist somehow..." << endl;
        return;
    }
    //don't do anything to selection if we're just finishing a move or rotate
    if (input->worldTarget==sceneData->aGizmo->xAxisGizmo)
        return;

    if (sceneData->actorMenu && sceneData->actorMenu->listButton.size()>0)
        return;

    //can select ground from layerInspector!
    if ( (other->name=="ground" || other->name=="grid" )&& input->hudTarget==NULL){
        input->deselectActors();
        return;
    }

    if (input->bShiftDown){                                                                //adding to selection
        bool foundInSelection=false;
        for (int i=0;i<(int)sceneData->selectedActors.size();i++){
            if (other==sceneData->selectedActors[i])
                foundInSelection=true;                                              //only add if we're not already in selection
        }
        if (!foundInSelection){
            if (other->groupID!="NULL" && !input->bCtrlDown){                        //if we're part of a group, add the whole group
                for (int i=0;i<(int)sceneData->actorList.size();i++){                //unless we hold down ctrl, then only add this one!
                    if (sceneData->actorList[i]->groupID==other->groupID){
                        sceneData->actorList[i]->bSelected=true;
                        sceneData->selectedActors.push_back(sceneData->actorList[i]);
                        }
                    }
                }
            else{
                other->bSelected=true;
                sceneData->selectedActors.push_back(other);                              //no CTRL or no group, but SHIFT down, so should add
                }
            }
        }
    else{
        bool foundInSelection=false;
        for (int i=0;i<(int)sceneData->selectedActors.size();i++){
            if (other==sceneData->selectedActors[i])
                foundInSelection=true;                                              //check if we're not already in selection
            }
        if (!foundInSelection && btn==MOUSEBTNLEFT){                                                     //if we're clicking on a actor not already in selection, you can go ahead
            input->deselectActors();
            if (other->groupID!="NULL" && !input->bCtrlDown){                            //if we're part of a group, select the whole group
                for (int i=0;i<(int)sceneData->actorList.size();i++){                    //unless we hold down CTRL, then only select me
                    if (sceneData->actorList[i]->groupID==other->groupID){
                        sceneData->actorList[i]->bSelected=true;
                        sceneData->selectedActors.push_back(sceneData->actorList[i]);
                        }
                    }
                }
            else{
                other->bSelected=true;
                sceneData->selectedActors.push_back(other);                                  //no CTRL or no Group
                }
            }
        else if (!foundInSelection && btn==MOUSEBTNRIGHT){                           //not in selection and clicked right
                other->bSelected=true;
                sceneData->selectedActors.push_back(other);
                }
        }
}

void SelectTool::makeGroup(){

        TextInputButton* messageWindow=new TextInputButton;
        messageWindow->name="GroupName:";
        messageWindow->bMessageWindow=true;
        messageWindow->parent=sceneData->selectedActors[0];
        messageWindow->buttonProperty="GROUPID";
        messageWindow->setLocation(Vector3f(renderer->windowX/2-200,renderer->windowY/2-50,0));
        messageWindow->scale=Vector3f(100,50,1);
        messageWindow->sceneShaderID="buttonColor";
        input->focusButton=messageWindow;
        messageWindow->level=100;
        messageWindow->setup();
        messageWindow->bDrawName=true;
        messageWindow->buttonColor=COLOR_GREEN;
        sceneData->buttonList.push_back(messageWindow);
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
        for (int i=0;i<(int)sceneData->selectedActors.size();i++){
            myPrefab->LinkEndChild(sceneData->selectedActors[i]->saveAsPrefab(myPrefab));
        }

        //save to disk
        TiXmlDocument doc;
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        doc.LinkEndChild( decl );
        doc.LinkEndChild(myPrefab);
        string stringName="resources/prefabs/"+prefabName+".prefab";
        doc.SaveFile(stringName);

        for (int i=0;i<(int)sceneData->prefabs.size();i++){
            if (sceneData->prefabs[i]==prefabName+".prefab");
                return;
        }
        sceneData->prefabs.push_back(prefabName+".prefab");

}

void SelectTool::pasteSelected(){

    if (!clipboard){
        cout << "Nothing has been cut/copied - cannot paste!"<< endl;
        return;
    }
    TiXmlElement* element=clipboard->FirstChildElement( "Actor" );
    string myType;
    int listPos=sceneData->actorList.size();
    for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
    {
        cout << "next element: " << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Actor * A=sceneData->actorInfo[myType].actorReference;
        A->create();
    }


    //then load all properties - for referencing reasons
    element=clipboard->FirstChildElement( "Actor" );
    for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
    {
        Actor* A=sceneData->actorList[listPos];
        myType=element->GetText();
        cout << "Loading property type: " << myType << endl;
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        A->load(element);
        A->bSelected=false; //just pasted, we are not really selected...
        listPos++;

        if (!input->bShiftDown)
            A->setLocation(input->mouse3D);

        A->setup();
    }
}

void SelectTool::cutSelected(){

    if (clipboard)
        delete clipboard;

    clipboard = new TiXmlElement( "Moviesandbox" );

    cout << "cutting..." << endl;
    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
        TiXmlElement* saveData=sceneData->selectedActors[i]->save(clipboard);
        clipboard->LinkEndChild(saveData);
    }

    for (int i=(int)sceneData->selectedActors.size()-1;i>-1;i--){
        sceneData->selectedActors[i]->remove();
    }
	//just to make sure...
	input->worldTarget=NULL;
}

void SelectTool::copySelected(){

    if (clipboard)
        delete clipboard;
    cout << "copying..." << endl;

    clipboard = new TiXmlElement( "Moviesandbox" );

    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
         TiXmlElement* saveData=sceneData->selectedActors[i]->save(clipboard);
        clipboard->LinkEndChild(saveData);
    }
}

void SelectTool::duplicateSelected(){

    cout << "duplicating..." << endl;
    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
        const std::type_info* myType=&(typeid(*(sceneData->selectedActors[i])));
        Actor * A=sceneData->actorInfo[myType->name()].actorReference;
        A->create();

        TiXmlElement * root = new TiXmlElement( "Moviesandbox" );
        A=sceneData->actorList[sceneData->actorList.size()-1];
        A->load(sceneData->selectedActors[i]->save(root));
        delete root;
        A->bSelected=false;                                             //flip selection when copying!
        A->setup();
        }
}
