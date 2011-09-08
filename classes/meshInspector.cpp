

#include "meshInspector.h"
#include "renderer.h"
#include "input.h"
#include "assignButton.h"
#include "colladaLoader.h"
#include "spriteMeshLoader.h"

MeshInspector::MeshInspector(){

    listColumns=4;
    level=0;
    listDisplaySize=250;
    textureID="icon_props";
}

MeshInspector::~MeshInspector(){}


void MeshInspector::setup(){

    Inspector::setup();
    tabs.clear();
    tabs.push_back( new MeshTab(this) );
    tabs.push_back( new TextureTab(this) );
}

void MeshInspector::createInspectorButtons(){

    AssignButton* importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    importButton->name="import";
    importButton->color=Vector4f(0.8,0.8,0.8,1.0);
    importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);


    AssignButton* tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+60.0f,location.y, 0.0f));
    tabButton->name="tabOne";
    tabButton->color=Vector4f(0.8,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+90.0f,location.y, 0.0f));
    tabButton->name="tabTwo";
    tabButton->color=Vector4f(0.8,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);


}

void MeshInspector::refreshList(){

    if (listButton.size()<sceneData->vboList.size()){
        tabs[currentTab]->assembleList();
    }
}


void MeshInspector::MeshTab::assembleList(){

        cout << "assembling mesh list..." << endl;
        std::map <std::string, MeshData*>::iterator it;
        int i=0;

        if (mine->scrollBar){
            mine->scrollBar->remove();
            mine->scrollBar=NULL;
        }


        /*
        for (int i=0; i<(int)mine->listButton.size();i++){
            mine->listButton[i]->bPermanent=false;
            mine->listButton[i]->deselect(0);
        }
        */

        for (int i=0; i<(int)mine->listButton.size();i++){
            mine->listButton[i]->remove();
            //mine->listButton[i]->deselect(0);
        }


        cout << "old mesh list size : " << mine->listButton.size() << endl;

        mine->listButton.clear();

        for ( it=mine->sceneData->vboList.begin() ; it != mine->sceneData->vboList.end(); it++ ){

            if (it->second && it->first!="NULL"){

                mine->sceneData->actorInfo["12AssignButton"].actorReference->create();
                mine->listButton.push_back(mine->sceneData->buttonList.back());

                mine->listButton[i]->name="string " + it->first;
                mine->listButton[i]->buttonProperty="VBOMESHID";
                mine->listButton[i]->textureID="icon_base";
                mine->listButton[i]->level=mine->level+1;
                mine->listButton[i]->bDrawName=true;
                mine->listButton[i]->color=Vector4f(1,1,1,1.0);
                mine->listButton[i]->bPermanent=true;
                mine->listButton[i]->bDragable=true;
                mine->listButton[i]->parent=mine;
                if (mine->listWidth>0)
                    mine->listButton[i]->scale.x=mine->listWidth;
                if (mine->listHeight>0)
                    mine->listButton[i]->scale.y=mine->listHeight;

                mine->listButton[i]->setup();
                mine->placeButton(i,i);
                //set this because we want to drag buttons around!
                mine->listButton[i]->initialLocation=mine->listButton[i]->location;

				cout << "create new mesh list button: " << mine->listButton[i]->name << endl;
                i++;
            }
        }


    cout << "our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void MeshInspector::MeshTab::trigger(MsbObject* other){



       for (int i=0;i<(int)mine->listButton.size();i++){
            mine->listButton[i]->color=COLOR_WHITE;
            if (other==mine->listButton[i]){
                ///set vboMeshID of worldTarget to
                if (mine->input->worldTarget && mine->input->worldTarget!=mine->sceneData->grid && mine->input->worldTarget->name!="ground"){
                    Actor* a=mine->input->worldTarget;
                    //special case for bones! Assign to parent character if present...
                    BoneActor* b = dynamic_cast<BoneActor*>(a);
                    if (b && b->parent)
                        a=(Actor*)b->parent;

                    ///setting the vboMesh
                    a->memberFromString(&a->property["VBOMESHID"], mine->listButton[i]->name);

                    //figure out if we should change the shader too!
                    if (mine->sceneData->vboList[a->vboMeshID]->bIsSkeletal)
                        a->sceneShaderID="skeletal";
                    //now, also reset the actor
                    a->reset();
                }
                ///but always set the brushes mesh id!
                mine->sceneData->brush->memberFromString(&mine->sceneData->brush->property["VBOMESHID"], mine->listButton[i]->name);
                mine->listButton[i]->color=COLOR_RED;
            }
        }

    if (other->name=="import"){

        //TODO: also copy to project folder!

        string filename=sceneData->openFileDialog();
        //check for file ending
        size_t found;
        found=filename.find(".dae");
        if (found!=string::npos){
            //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION);
            string meshID=filename.substr(found+1);
            sceneData->colladaLoader->loadColladaMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("ColladaMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(myElement);

        }
        found=filename.find(".DAE");
        if (found!=string::npos){
            cout << "found a COLLADA mesh!" << filename << endl;
           //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION);
            string meshID=filename.substr(found+1);
            sceneData->colladaLoader->loadColladaMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("ColladaMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(myElement);
        }
        found=filename.find(".spriteMesh");
        if (found!=string::npos){
            //generate meshID
            found=filename.find_last_of(DIRECTORY_SEPARATION);
            string meshID=filename.substr(found+1);
            sceneData->spriteMeshLoader->loadSpriteMesh(filename, meshID);

            //open my.library and append this mesh!
            TiXmlElement* myElement = new TiXmlElement("SpriteMesh");
            myElement->SetAttribute("meshID",meshID);
            myElement->SetAttribute("meshFilename",filename);
            sceneData->addToLibrary(myElement);
        }
    }


}


void MeshInspector::TextureTab::assembleList(){

    for (int i=0;i<(int)mine->listButton.size();i++){
        mine->listButton[i]->remove();
    }
    mine->listButton.clear();

    if (mine->scrollBar){
        mine->scrollBar->remove();
        mine->scrollBar=NULL;
    }

        cout << "creating list..." << endl;
        std::map <std::string, textureObject*>::iterator it;
        int i=0;

        for ( it=mine->sceneData->textureList.begin() ; it != mine->sceneData->textureList.end(); it++ ){

            mine->sceneData->actorInfo["20PropertyAssignButton"].actorReference->create();
            mine->listButton.push_back(mine->sceneData->buttonList.back());

            mine->listButton[i]->name="string " + it->first;
            mine->listButton[i]->buttonProperty="TEXTUREID";
            mine->listButton[i]->textureID="icon_base";
            mine->listButton[i]->level=mine->level+1;
            mine->listButton[i]->bDrawName=false;
            mine->listButton[i]->color=Vector4f(1,1,1,1.0);
            mine->listButton[i]->textureID=it->first;
            mine->listButton[i]->bPermanent=true;
            mine->listButton[i]->bDragable=true;
            if (mine->listWidth>0)
                mine->listButton[i]->scale.x=mine->listWidth;
            if (mine->listHeight>0)
                mine->listButton[i]->scale.y=mine->listHeight;

            mine->listButton[i]->setup();
            mine->placeButton(i,i);
            //set this because we want to drag buttons around!
            mine->listButton[i]->initialLocation=mine->listButton[i]->location;

            i++;
        }

    cout << "TextureInspector: our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}



void MeshInspector::trigger(MsbObject* other){


    Inspector::trigger(other);

    if (other->name=="tabOne"){
        currentTab=0;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="tabTwo"){
        currentTab=1;
        tabs[currentTab]->assembleList();
    }


}

void MeshInspector::create(){sceneData->addButton(this);}
