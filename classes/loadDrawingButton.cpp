

#include "loadDrawingButton.h"
#include "renderer.h"
#include "input.h"
#include "sceneData.h"
#include "drawingWidget.h"
#include "brush.h"


LoadDrawingButton::LoadDrawingButton(){

textureID="icon_load";
listWidth=20;
listHeight=20;

listWidth=200;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listShader="color";
}

LoadDrawingButton::~LoadDrawingButton(){}

void LoadDrawingButton::clickedLeft(){

    //input->deselectButtons(0);
    sceneData->getAllDrawings();
    MsbObject * myParent=parent;
    listDisplayMode=3;

    LoadButton::clickedLeft();

    parent=myParent;
    bDrawName=false;
}

void LoadDrawingButton::assembleLoadList(){

    // in the drawing widget, open the list up towards the top
    // in the system menu, do the standard right-down thing
    if (parent->isA("DrawingWidget"))
      {
      parent=NULL;
      for (unsigned int i=0;i<sceneData->savedDrawings.size();i++)
        {
        listType.push_back("12AssignButton");
        listName.push_back(sceneData->savedDrawings[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_base");
        }
      }
    else
      {
      listDisplayMode=0;
      parent=NULL;
      /*
        if (buttonProperty==sceneData->savedDrawingDirName)
          for (unsigned int i=0;i<sceneData->savedDrawings.size();i++)
            {
            listType.push_back("19CreateDrawingButton");
            listName.push_back(sceneData->savedDrawings[i]);
            listProp.push_back(buttonProperty);
            listIcon.push_back("icon_props");
            }
        else
          for (unsigned int i=0;i<sceneData->savedCharacters.size();i++)
            {
            listType.push_back("19CreateDrawingButton");
            listName.push_back(sceneData->savedCharacters[i]);
            listProp.push_back(buttonProperty);
            listIcon.push_back("icon_character");
            }
      */
      }
}

void LoadDrawingButton::placeScrollBar(){

    scrollBar->setLocation( Vector3f( location.x-17, location.y- scale.y -( maxListItems * listButton[0]->scale.y ), 0) );
    scrollBar->bFlipValue=true;

}

void LoadDrawingButton::trigger(MsbObject* other){

    ListButton::trigger(other);
    if (other==scrollBar)
        return;

    if (parent->isA("DrawingWidget"))
      loadFile(other->name);;
    input->deselectButtons(0);
}

void LoadDrawingButton::loadFile(string filename){

    //todo: clear previous drawing!
    //if (DrawingWidget::brush)
    //  DrawingWidget::brush->setLocation(newLocation);
    std::string stringName=sceneData->startProject + "/";
    stringName.append(filename);

    TiXmlDocument doc( stringName );

    if (!doc.LoadFile()) return;

    cout << "Loading Drawing..." <<endl;

    TiXmlHandle hDoc(&doc);
    TiXmlElement * myInfo;

    myInfo=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!myInfo) return;

    TiXmlHandle hRoot(0);
    // save this for later
    hRoot=TiXmlHandle(myInfo);

    //***********************************************************************
    //Assemble Actor List
    //***********************************************************************


      TiXmlElement* element=hRoot.FirstChild( "Actor" ).Element();
      string myType;
      for( ; element!=NULL; element=element->NextSiblingElement())
        {
        //cout << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Actor * A=sceneData->actorInfo[myType].actorReference;
        A->create();
        A=sceneData->actorList.back();
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        A->load(element);
        A->setup();
        if (sceneData->brush)
          sceneData->brush->drawing=(SkeletalActor*) A;
        }
}

void LoadDrawingButton::create(){sceneData->addButton(this);}
