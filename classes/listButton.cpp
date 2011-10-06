#include "listButton.h"
#include "renderer.h"
#include "input.h"

ListButton::ListButton(){

listDisplayMode=0;

radius=0;
maxRadius=1;
animSpeed=0.5;
listWidth=0;
listHeight=0;
listOffsetX=0;
listOffsetY=0;

bRelativeListLocation=true;
listLoc=Vector3f(0,0,0);
listButtonSpacing=Vector3f(2,2,0);

scrollSize=128.0;
listDisplaySize=300.0;

listColumns=1;
scrollBar=NULL;
backgroundButton=NULL;

bPermanentList=false;
bDrawListNames=false;
bListOpen=false;

listColor=Vector4f(1,1,1,1);
listShader="texture";
}

ListButton::~ListButton(){}

void ListButton::setup(){
    BasicButton::setup();
}

void ListButton::clickedLeft(){

    cout << "clicked! " << name << endl;


  BasicButton::clickedLeft();

  //destroy all buttons that have higher depth than mine!
  input->deselectButtons(level);

  if (listButton.size()==0 || bPermanentList){
    assembleList();
    bListOpen=true;
  }

}

void ListButton::assembleList(){

    listOffsetX=0;

    Vector3f startListLoc=listLoc;

    if (bPermanentList && (int)listButton.size()>0){
        for (int i=0;i<listButton.size();i++)
                listButton[i]->bHidden=false;
        return;
    }

    for (int i=0;i<(int)listType.size();i++){
        cout << "creating list..." << endl;

        sceneData->actorInfo[listType[i]].actorReference->create();
        listButton.push_back(sceneData->buttonList.back());

        cout << "made button " << listButton[i]->name << endl;

        if ((int)listName.size()>i && listName[i]!="")
        listButton[i]->name=listName[i];

        if ((int)listProp.size()>i && listProp[i]!="")
        listButton[i]->buttonProperty=listProp[i];

        if ((int)listIcon.size()>i && listIcon[i]!="")
        listButton[i]->textureID=listIcon[i];

        //now allowing string parenting!
        //special stuff: RENDERER - parents to the renderer
        //special stuff: THIS - parents to the creating Listbutton (literally, this)
        //special stuff: PARENT - parents to the parent of the creating Listbutton
        //special stuff: SELECTED - parents to the first Selected Actor
        //special stuff: you can also directly pass an Actor reference here!

        if ((int)listParent.size()>i && listParent[i]!=""){
            if (listParent[i]=="RENDERER")
                listButton[i]->parent=renderer;
            else if (listParent[i]=="THIS")
                listButton[i]->parent=this;
            else if (listParent[i]=="PARENT")
                listButton[i]->parent=parent;
            else if (listParent[i]=="SELECTED")
                listButton[i]->parent=sceneData->selectedActors[0];
            else
                listButton[i]->parent=readActor((char*)listParent[i].c_str());

        }else{

            if (parent)
                listButton[i]->parent=parent;
            else
                listButton[i]->parent=this;
        }
        listButton[i]->level=level+1;
        listButton[i]->bDrawName=bDrawListNames;
        listButton[i]->color=listColor;
        listButton[i]->sceneShaderID=listShader;

        if (i>0 && listButton[i-1]->bIndividualListSize){
            listLoc.y+=listButton[i-1]->scale.y-listHeight;// + listButtonSpacing.y;
        }
        if (!listButton[i]->bIndividualListSize){
            if (listWidth>0)
                listButton[i]->scale.x=listWidth;
            if (listHeight>0)
                listButton[i]->scale.y=listHeight;
        }

        if (bPermanentList)
        listButton[i]->bPermanent=true;



        placeButton(i,i);
        listButton[i]->setup();
        cout << "placing..." << endl;
    }

    listLoc=startListLoc;


    if (listButton.size()>0)
        listSize.y=listButton[listButton.size()-1]->location.y+listButton[listButton.size()-1]->scale.y - location.y;
    else
        listSize.y=0;

    if (!scrollBar && listSize.y > listDisplaySize)
        createScrollBar();
}

void ListButton::createScrollBar(){

    //scrollButton
      scrollBar=new SliderButton;
      sceneData->buttonList.push_back(scrollBar);
      scrollBar->sceneShaderID="color";
      scrollBar->parent=this;
      scrollBar->scale.x=8;
      scrollBar->scale.y=scrollSize;
      scrollBar->level=level+1;
      scrollBar->color=Vector4f(0.7,0.7,0.7,1.0);
      scrollBar->bPermanent=true;
      placeScrollBar();

      //scrollBar functionalitty:
      trigger(scrollBar);

}

void ListButton::placeScrollBar(){

		scrollBar->setLocation( Vector3f( location.x+scale.x+listButton[0]->scale.x+2, scrollBar->location.y=location.y, 0 ) );
}


void ListButton::placeButton(int buttonNumber, int drawPosition){

    bool bCornered=false;
    float sinePart=2*M_PI;
    float circlePos=float(drawPosition)/float(listType.size());
    maxRadius=1.1*scale.y;


    if (listHeight==0)
        listHeight=listButton[buttonNumber]->scale.y;
    if (listWidth==0)
        listHeight=listButton[buttonNumber]->scale.x;


    Vector3f loc;
    switch(listDisplayMode){


        case 0:                     //to the right and down
            if (bRelativeListLocation){
                loc.x=location.x+listLoc.x+scale.x+listButtonSpacing.x;
                loc.y=location.y+listLoc.y+drawPosition*(listHeight+listButtonSpacing.y) + listOffsetY;
            }else{
                loc.x=listLoc.x;
                loc.y=listLoc.y+drawPosition*(listHeight+listButtonSpacing.y)  + listOffsetY;
            }
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 1:                     //weird thing for brush-size

            loc.x=location.x+(drawPosition+1)*(listWidth+2);
            loc.y=location.y-(listHeight-scale.y);
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 2:                       //circular
            if (bCornered)
             sinePart=M_PI;
            loc.x=location.x+radius*sin(circlePos*sinePart);
            loc.y=location.y+radius*cos(circlePos*sinePart);
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 3:                       //straight upwards
            if (bRelativeListLocation){
                loc.x=listLoc.x+location.x;
                loc.y=listLoc.y+location.y-scale.y-drawPosition*(listHeight+listButtonSpacing.y) - listOffsetY;
            }else{
                loc.x=listLoc.x;
                loc.y=listLoc.y-scale.y-drawPosition*(listHeight+listButtonSpacing.y) - listOffsetY;
            }
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 5:                     //straight Down
            if (bRelativeListLocation){
                loc.x=location.x+listLoc.x+listButtonSpacing.x;
                loc.y=location.y+listLoc.y+drawPosition*(listHeight+listButtonSpacing.y) + listOffsetY+ scale.y;
            }else{
                loc.x=listLoc.x;
                loc.y=listLoc.y+drawPosition*(listHeight+listButtonSpacing.y)  + listOffsetY + scale.y;
            }
            listButton[buttonNumber]->setLocation(loc);
            break;


        case 4:                       //box, right - used for texture browser
            //find row
            int row=drawPosition/listColumns;
            //find column
            int column = drawPosition%listColumns;

            if (bRelativeListLocation){
                loc.x=listLoc.x + location.x + scale.x + column * (listWidth+listButtonSpacing.x);
                loc.y=listLoc.y + location.y + row * (listHeight+listButtonSpacing.y) + listOffsetY;
            }else{
                loc.x=listLoc.x + scale.x + column * (listWidth+listButtonSpacing.x);
                loc.y=listLoc.y + row * (listHeight+listButtonSpacing.y) + listOffsetY;
            }
            listButton[buttonNumber]->setLocation(loc);
            break;


    }
    listButton[buttonNumber]->initialLocation=listButton[buttonNumber]->location;
}

void ListButton::update(double deltaTime){

    if (listDisplayMode==2 && radius<maxRadius){
        radius+=animSpeed*deltaTime;
        radius=min(radius,maxRadius);

        for (int i=0;i<(int)listType.size();i++)
            placeButton(i,i);
    }
}

void ListButton::deselect(int depth){


    bListOpen=false;

    //get rid of scrollButton if we're not permanent!
    if (scrollBar){
        scrollBar->bPermanent=false;
        scrollBar->remove();
        scrollBar=NULL;
    }

    if (bPermanentList){
        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bHidden=true;
        }

    }

    else if (listButton.size()>0){// && !listButton[0]->bPermanent){
      cout << "clearing list..." << endl;
      listButton.clear();
    }


    //remove myself from screen if I am not a menu item!
    //will be determined by depth
    BasicButton::deselect(depth);
}

void ListButton::trigger(MsbObject* other){

    if (scrollBar && other==scrollBar){

        if (listButton.size()==0)
            return;
        //adjust positions
        //hide buttons

        Vector3f oldListLoc=listLoc;
        listLoc.y-=(listSize.y-listDisplaySize)*scrollBar->sliderValue;

        //listOffsetY=20;

        for (int i=0;i<(int)listButton.size();i++){

            //if sliderValue is 1, begin is size()-maxDraw
            //if sliderValue is 0, begin is 0

            //recalculate listOffset for buttons that are individually sized
            if (i>0 && listButton[i-1]->bIndividualListSize){
                //listOffsetY+=listButton[i-1]->scale.y-listHeight;
                listLoc.y+=listButton[i-1]->scale.y-listHeight;
            }


            //do not re-position button if we're dragging it!
            if (input->dragButton!=listButton[i]){
                placeButton(i,i);

                if (listButton[i]->location.y<location.y + listOffsetY || listButton[i]->location.y>location.y+listDisplaySize)// || listButton[i]->location.y> location.y + listSize.y)
                    listButton[i]->bHidden=true;
                else
                    listButton[i]->bHidden=false;

            }

        }

        listLoc=oldListLoc;

    return;
    }

    if (other->name=="exit")
        exit(0);
}

void ListButton::create(){
sceneData->addButton(this);
std::cout << "creating a listButton!";
}
