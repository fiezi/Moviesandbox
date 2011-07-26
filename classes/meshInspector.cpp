

#include "meshInspector.h"
#include "renderer.h"
#include "input.h"
#include "assignButton.h"
#include "colladaLoader.h"
#include "spriteMeshLoader.h"

MeshInspector::MeshInspector(){

    listColumns=4;
    level=0;
    textureID="icon_props";
}

MeshInspector::~MeshInspector(){}

void MeshInspector::createInspectorButtons(){

    AssignButton* importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    importButton->name="import";
    importButton->color=Vector4f(0.8,0.8,0.8,1.0);
    importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);
}

void MeshInspector::refreshList(){

    if (listButton.size()<sceneData->vboList.size()){
        assembleList();
    }
}


void MeshInspector::assembleList(){

        cout << "assembling mesh list..." << endl;
        std::map <std::string, MeshData*>::iterator it;
        int i=0;
        for (int i=0; i<(int)listButton.size();i++){
            listButton[i]->bPermanent=false;
            listButton[i]->deselect(0);
        }
        cout << "old mesh list size : " << listButton.size() << endl;

        listButton.clear();

        for ( it=sceneData->vboList.begin() ; it != sceneData->vboList.end(); it++ ){

            if (it->second && it->first!="NULL"){

                sceneData->actorInfo["12AssignButton"].actorReference->create();
                listButton.push_back(sceneData->buttonList.back());

                listButton[i]->name="string " + it->first;
                listButton[i]->buttonProperty="VBOMESHID";
                listButton[i]->textureID="icon_base";
                listButton[i]->level=level+1;
                listButton[i]->bDrawName=true;
                listButton[i]->color=Vector4f(1,1,1,1.0);
                listButton[i]->bPermanent=true;
                listButton[i]->bDragable=true;
                listButton[i]->parent=this;
                if (listWidth>0)
                    listButton[i]->scale.x=listWidth;
                if (listHeight>0)
                    listButton[i]->scale.y=listHeight;

                listButton[i]->setup();
                placeButton(i,i);
                //set this because we want to drag buttons around!
                listButton[i]->initialLocation=listButton[i]->location;

				cout << "create new mesh list button: " << listButton[i]->name << endl;
                i++;
            }
        }


    cout << "our Button list is: "<< listButton.size() <<" elements long..." << endl;
}


void MeshInspector::trigger(MsbObject* other){


    for (int i=0;i<(int)listButton.size();i++){
		listButton[i]->color=COLOR_WHITE;
        if (other==listButton[i]){
            ///set vboMeshID of worldTarget to
            if (input->worldTarget && input->worldTarget!=sceneData->grid && input->worldTarget->name!="ground"){
                Actor* a=input->worldTarget;
                //special case for bones! Assign to parent character if present...
                BoneActor* b = dynamic_cast<BoneActor*>(a);
                if (b && b->parent)
                    a=(Actor*)b->parent;

                ///setting the vboMesh
                a->memberFromString(&a->property["VBOMESHID"], listButton[i]->name);

				//figure out if we should change the shader too!
				if (sceneData->vboList[a->vboMeshID]->bIsSkeletal)
					a->sceneShaderID="skeletal";
                //now, also reset the actor
                a->reset();
            }
            ///but always set the brushes mesh id!
            sceneData->brush->memberFromString(&sceneData->brush->property["VBOMESHID"], listButton[i]->name);
			other->color=COLOR_RED;
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

void MeshInspector::create(){sceneData->addButton(this);}
