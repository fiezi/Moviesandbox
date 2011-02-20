#include "saveDrawingButton.h"
#include "input.h"
#include "renderer.h"
#include "drawingWidget.h"

SaveDrawingButton::SaveDrawingButton(){

listDisplayMode=3;
listHeight=20;
textureID="icon_save";

listWidth=200;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listShader="color";
}

SaveDrawingButton::~SaveDrawingButton(){}

void SaveDrawingButton::clickedLeft(){

    input->deselectButtons(0);
    Actor * myParent=parent;

    input->getAllDrawings();

    SaveButton::clickedLeft();
    parent=myParent;
}

void SaveDrawingButton::assembleSaveList(){

      for (unsigned int i=0;i<input->savedDrawings.size();i++)
        {
        listType.push_back("12AssignButton");
        listName.push_back(input->savedDrawings[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_base");
        }
}


void SaveDrawingButton::placeScrollBar(){

    scrollBar->location.x=location.x-17;
    scrollBar->location.y=location.y- scale.y -( maxListItems * listButton[0]->scale.y );
    scrollBar->bFlipValue=true;


}

void SaveDrawingButton::trigger(Actor * other){

    if (other==scrollBar)
        return;

    SaveButton::trigger(other);
}

void SaveDrawingButton::saveFile(string filename){

cout << "saving a drawing..." << endl;

TiXmlDocument doc;
TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
doc.LinkEndChild( decl );
TiXmlElement * root = new TiXmlElement( "Drawing" );
doc.LinkEndChild( root );

string saveString;
root->LinkEndChild( renderer->brush->drawing->save(root) );
saveString=input->savedDrawingDirName;

saveString.append(filename);
cout << "saving filename: " << saveString << endl;
doc.SaveFile( saveString );
}

void SaveDrawingButton::create(){renderer->addButton(this);}
