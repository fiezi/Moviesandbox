

#include "textureInspector.h"
#include "renderer.h"
#include "input.h"
#include "assignButton.h"

TextureInspector::TextureInspector(){

    bOpen=false;
    bPermanent=true;
    initialLocation=location;
    tabWidth=350;
    listHeight=32;
    listWidth=32;
    listDisplayMode=4;
    listColumns=8;
    level=0;
}

TextureInspector::~TextureInspector(){}

void TextureInspector::createInspectorButtons(){

    AssignButton* importButton= new AssignButton;
    importButton->parent=this;
    sceneData->buttonList.push_back(importButton);
    importButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    importButton->name="import";
    importButton->color=Vector4f(0.8,0.8,0.8,1);
    importButton->bDrawName=true;
    inspectorButtons.push_back(importButton);

}

void TextureInspector::refreshList(){
    if (sceneData->textureList.size()!=listButton.size()){

        assembleList();
        clickedLeft();
        clickedLeft();

    }
}

void TextureInspector::assembleList(){

    for (int i=0;i<(int)listButton.size();i++){
        listButton[i]->remove();
    }
    listButton.clear();

        cout << "creating list..." << endl;
        std::map <std::string, textureObject*>::iterator it;
        int i=0;

        for ( it=sceneData->textureList.begin() ; it != sceneData->textureList.end(); it++ ){

            sceneData->actorInfo["20PropertyAssignButton"].actorReference->create();
            listButton.push_back(sceneData->buttonList.back());

            listButton[i]->name="string " + it->first;
            listButton[i]->buttonProperty="TEXTUREID";
            listButton[i]->textureID="icon_base";
            listButton[i]->level=level+1;
            listButton[i]->bDrawName=false;
            listButton[i]->color=Vector4f(1,1,1,1.0);
            listButton[i]->textureID=it->first;
            listButton[i]->bPermanent=true;
            listButton[i]->bDragable=true;
            if (listWidth>0)
                listButton[i]->scale.x=listWidth;
            if (listHeight>0)
                listButton[i]->scale.y=listHeight;

            listButton[i]->setup();
            placeButton(i,i);
            //set this because we want to drag buttons around!
            listButton[i]->initialLocation=listButton[i]->location;

            i++;
        }

    cout << "TextureInspector: our Button list is: "<< listButton.size() <<" elements long..." << endl;
}

void TextureInspector::clickedRight(){
}


void TextureInspector::trigger(MsbObject* other){

    if (other->name=="import"){
        string filename=sceneData->openFileDialog();

        cout <<"importing..." << filename << endl;

        size_t found;
        found=filename.find(".tga");
        if (found == string::npos){
            found=filename.find(".TGA");

            if (found==string::npos){
                cout << "Not the right image format, sorry!" << endl;
                return;
            }
        }

        //generate meshID
        found=filename.find_last_of(DIRECTORY_SEPARATION);
        string texID=filename.substr(found+1);

        renderer->LoadTextureTGA(filename,true,true,texID);
        TiXmlElement * texElement = new TiXmlElement("Texture");
        texElement->SetAttribute("textureID",texID);
        texElement->SetAttribute("filename",filename);
        texElement->SetAttribute("bAlpha",1);
        texElement->SetAttribute("bWrap",1);
        sceneData->addToLibrary(texElement);
        free(texElement);
    }
}

void TextureInspector::create(){sceneData->addButton(this);}
