

#include "menuBar.h"
#include "renderer.h"
#include "input.h"
#include "spriteMeshLoader.h"
MenuBar::MenuBar(){

    registerProperties();
    scale.x=64;
    scale.y=16;
    color=Vector4f(0.6,0.6,0.6,0.8);
    name="MenuBar";

    bToggledMenu=false;
    activeMenu=-1;

}

MenuBar::~MenuBar(){}


void MenuBar::setup(){

    BasicButton::setup();

    menuColor=sceneData->menuBarColor;
    buttonColor=sceneData->menuBarColor;
    mouseOverColor=sceneData->menuBarColor;

    ListButton* lbut=new ListButton;

    lbut->location.x= 0;
    lbut->scale.x=64;
    lbut->scale.y=16;
    lbut->sceneShaderID="buttonColor";
	lbut->setTextureID("icon_flat");
    lbut->level=0;
    lbut->bPermanent=true;
    lbut->bPermanentList=true;
    lbut->bPartOfMenu=true;
    lbut->name="File";
    lbut->bDrawName=true;
    lbut->setLocation(lbut->location);
    lbut->parent=this;
    lbut->bDrawListNames=true;
    lbut->listDisplayMode=5;//straight down
    lbut->listShader="buttonColor";
    lbut->listColor=menuColor;
    //fill list and set to parent!
    lbut->listWidth=128;
    lbut->listHeight=16;
    lbut->listButtonSpacing=Vector3f(0,0,0);

    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("New Scene");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("NEWSCENE");

    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("Load Scene...");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("LOADSCENELIST");
    //lbut->listProp.push_back("SCENE");

    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("Save Scene");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("SAVESCENE");

    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("Save Scene As...");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("SAVESCENEAS");

    lbut->listType.push_back("11BasicButton");
    lbut->listName.push_back("----------------");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("NULL");


    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("New Project");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("NEWPROJECT");

    lbut->listType.push_back("10LoadButton");
    lbut->listName.push_back("Load Project");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("LOADPROJECT");

    lbut->listType.push_back("11BasicButton");
    lbut->listName.push_back("----------------");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("NULL");

    lbut->listType.push_back("12WindowButton");
    lbut->listName.push_back("Preferences");
    lbut->listIcon.push_back("icon_system");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("11BasicButton");
    lbut->listName.push_back("----------------");
    lbut->listParent.push_back("PARENT");
    lbut->listProp.push_back("NULL");


    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Quit");
    lbut->listParent.push_back("PARENT");

    sceneData->buttonList.push_back(lbut);
    menus.push_back(lbut);
    lbut->setup();
    lbut->buttonColor=menuColor;

    /** EDIT MENU  **/

    lbut=new ListButton;
    lbut->location.x= 64;
    lbut->scale.x=64;
    lbut->scale.y=16;
    lbut->sceneShaderID="buttonColor";
	lbut->setTextureID("icon_flat");
    lbut->level=0;
    lbut->bPermanent=true;
    lbut->bPermanentList=true;
    lbut->bPartOfMenu=true;
    lbut->name="Edit";
    lbut->bDrawName=true;
    lbut->setLocation(lbut->location);
    lbut->parent=this;
    lbut->bDrawListNames=true;
    lbut->listDisplayMode=5;//straight down
    lbut->listShader="buttonColor";
    lbut->listColor=menuColor;
    //fill list and set to parent!
    lbut->listWidth=128;
    lbut->listHeight=16;
    lbut->listButtonSpacing=Vector3f(0,0,0);

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Cut");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Copy");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Paste");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Delete");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Group");
    lbut->listParent.push_back("PARENT");
    sceneData->buttonList.push_back(lbut);
    menus.push_back(lbut);
    lbut->setup();
    lbut->buttonColor=menuColor;

    /** DRAW MENU  **/

    lbut=new ListButton;
    lbut->location.x= 128;
    lbut->scale.x=64;
    lbut->scale.y=16;
    lbut->sceneShaderID="buttonColor";
	lbut->setTextureID("icon_flat");
    lbut->level=0;
    lbut->bPermanent=true;
    lbut->bPermanentList=true;
    lbut->bPartOfMenu=true;
    lbut->name="Draw";
    lbut->bDrawName=true;
    lbut->setLocation(lbut->location);
    lbut->parent=this;
     lbut->bDrawListNames=true;
    lbut->listDisplayMode=5;//straight down
    lbut->listShader="buttonColor";
    lbut->listColor=menuColor;
    //fill list and set to parent!
    lbut->listWidth=128;
    lbut->listHeight=16;
    lbut->listButtonSpacing=Vector3f(0,0,0);

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("ScaleZ 0.5");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("ScaleZ 2.0");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Merge Drawings");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Split Drawing");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Save Drawing As...");
    lbut->listParent.push_back("PARENT");
    sceneData->buttonList.push_back(lbut);
    menus.push_back(lbut);
    lbut->setup();
    lbut->buttonColor=menuColor;

   /** VIEW MENU  **/

    lbut=new ListButton;
    lbut->location.x= 196;
    lbut->scale.x=64;
    lbut->scale.y=16;
    lbut->sceneShaderID="buttonColor";
	lbut->setTextureID("icon_flat");
    lbut->level=0;
    lbut->bPermanent=true;
    lbut->bPartOfMenu=true;
    lbut->bPermanentList=true;
    lbut->name="View";
    lbut->bDrawName=true;
    lbut->setLocation(lbut->location);
    lbut->parent=this;
     lbut->bDrawListNames=true;
    lbut->listDisplayMode=5;//straight down
    lbut->listShader="buttonColor";
    lbut->listColor=menuColor;
    //fill list and set to parent!
    lbut->listWidth=128;
    lbut->listHeight=16;
    lbut->listButtonSpacing=Vector3f(0,0,0);

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Toggle Nodes (F2)");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Color Only (F3)");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Toggle SSAO (F4)");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Toggle Lighting (F5)");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Toggle Normals (F6)");
    lbut->listParent.push_back("PARENT");

    lbut->listType.push_back("12AssignButton");
    lbut->listName.push_back("Toggle PostPro (F7)");
    lbut->listParent.push_back("PARENT");

    sceneData->buttonList.push_back(lbut);
    menus.push_back(lbut);
    lbut->setup();
    lbut->buttonColor=menuColor;

    /** TOOLS MENU **/
/*
    lbut=new ListButton;
    lbut->location.x= 256;
    lbut->scale.x=64;
    lbut->scale.y=16;
    lbut->sceneShaderID="buttonColor";
	lbut->setTextureID("icon_flat");
    lbut->level=0;
    lbut->bPermanent=true;
    lbut->bPartOfMenu=true;
    lbut->bPermanentList=true;
    lbut->name="Tools";
    lbut->bDrawName=true;
    lbut->setLocation(lbut->location);
    lbut->parent=this;
    sceneData->buttonList.push_back(lbut);
    menus.push_back(lbut);
    lbut->setup();
    lbut->buttonColor=menuColor;
*/
}

void MenuBar::update(double deltaTime){

    BasicButton::update(deltaTime);


    //make menus behave like menus
    if (bToggledMenu){


        //find menu that we currently hover over
        for (int i=0;i<menus.size();i++)
            if (input->hudTarget==menus[i])
                    activeMenu=i;


        //if we hover over a menu that is not opened
        if (activeMenu>=0 && !menus[activeMenu]->bListOpen){
            //close all other menus
            for (int i=0;i<menus.size();i++)
                if (menus[i]->bListOpen)
                    menus[i]->deselect(0);

            //open this one
            menus[activeMenu]->clickedLeft();
        }

    }
}

void MenuBar::mouseOver(){

    BasicButton::mouseOver();
}

void MenuBar::clickedLeft(){}
void MenuBar::clickedRight(){}

void MenuBar::focusClick(){

    BasicButton::focusClick();
}


void MenuBar::trigger(MsbObject* other){

    //activate menu if we clicked on menu item
    for (int i=0;i<menus.size();i++){
        if (other==menus[i]){
            bToggledMenu=true;
            activeMenu=i;
        }
    }

    if (other->name=="Quit"){

        exit(0);
    }

    //Edit Menu
    if (other->name=="Cut"){
        sceneData->selectTool->cutSelected();
    }

    if (other->name=="Copy"){
        sceneData->selectTool->copySelected();
    }

    if (other->name=="Paste"){
        sceneData->selectTool->pasteSelected();
    }

    if (other->name=="Duplicate"){
        sceneData->selectTool->duplicateSelected();
    }


    //Drawing Menu

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

        if (sceneData->selectedActors.size()<2){
            sceneData->makeWarningPopUp("You must have at least 2 drawings selected to merge them!", NULL);
            return;
        }
        sceneData->drawTool->mergeDrawings();
    }

    if (other->name== "Split Drawing"){

        if (sceneData->brush->selectedData.size()<2){
            sceneData->makeWarningPopUp("You must have Particles selected in order to split them from a drawing!", NULL);
            return;
        }
        sceneData->drawTool->splitDrawing();
    }

    //View Menu


    if (other->name=="Toggle Nodes (F2)"){

        renderer->bDrawNodes=!renderer->bDrawNodes;
    }

    if (other->name=="Color Only (F3)"){
        renderer->bDrawLighting=!renderer->bDrawLighting;
    }

    if (other->name=="Toggle SSAO (F4)"){
            renderer->bSSAO=!renderer->bSSAO;
    }

    if (other->name=="Toggle Lighting (F5)"){
            renderer->bDrawColor=!renderer->bDrawColor;
    }

    if (other->name=="Toggle Normals (F6)"){
            renderer->bDrawNormals=!renderer->bDrawNormals;
    }

    if (other->name=="Toggle PostPro (F7)"){
             renderer->bDOF=!renderer->bDOF;
    }

}


void MenuBar::deselect(int depth){

    BasicButton::deselect(depth);
    bToggledMenu=false;
}

void MenuBar::create(){sceneData->addButton(this);}
