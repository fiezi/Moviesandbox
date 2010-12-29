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

listColumns=1;
maxListItems=10;
beginListItem=0;
scrollBar=NULL;

bPermanentList=false;

listColor=Vector4f(1,1,1,1);
listShader="texture";
}

ListButton::~ListButton(){}

void ListButton::setup(){
BasicButton::setup();
}

void ListButton::clickedLeft(){

  BasicButton::clickedLeft();

  //destroy all buttons that have higher depth than mine!
  input->deselectButtons(level);

  if (listButton.size()==0){
  cout << "clicked!" << endl;
  assembleList();
  }

}

void ListButton::assembleList(){

  for (unsigned int i=0;i<listType.size();i++){
      cout << "creating list..." << endl;

      renderer->actorInfo[listType[i]].actorReference->create();
      listButton.push_back(renderer->buttonList.back());

      cout << "made button " << listButton[i]->name << endl;

      if (listName.size()>i && listName[i]!="")
        listButton[i]->name=listName[i];

      if (listProp.size()>i && listProp[i]!="")
        listButton[i]->buttonProperty=listProp[i];

      if (listIcon.size()>i && listIcon[i]!="")
        listButton[i]->textureID=listIcon[i];

      if (parent)
        listButton[i]->parent=parent;
      else
        listButton[i]->parent=this;
      listButton[i]->level=level+1;
      listButton[i]->bDrawName=bDrawName;
      listButton[i]->color=listColor;
      listButton[i]->sceneShaderID=listShader;

      listButton[i]->setup();

      if (bPermanentList)
        listButton[i]->bPermanent=true;

      placeButton(i,i);
      cout << "placing..." << endl;
    }

    if (!scrollBar)
        createScrollBar();
}

void ListButton::createScrollBar(){

  if ((int)listButton.size()>maxListItems){
    //scrollButton
      scrollBar=new SliderButton;
      renderer->buttonList.push_back(scrollBar);
      scrollBar->sceneShaderID="color";
      scrollBar->parent=this;
      scrollBar->scale.x=15;
      scrollBar->scale.y=listButton[0]->scale.y*min(maxListItems,(int)listButton.size())/listColumns;
      scrollBar->level=level+1;
      scrollBar->color=Vector4f(0.2,0.2,0.2,1.0);
      scrollBar->bPermanent=true;
      placeScrollBar();

      //scrollBar functionalitty:
      for (int i=maxListItems;i<(int)listButton.size();i++){
        listButton[i]->bHidden=true;
      }

      //trigger scrollBar once to initialise
      trigger(scrollBar);
  }
}

void ListButton::placeScrollBar(){

	if (listButton.size()>0){
		scrollBar->setLocation( Vector3f( location.x+scale.x+listButton[0]->scale.x+2, scrollBar->location.y=location.y, 0 ) );
	}
}


void ListButton::placeButton(int buttonNumber, int drawPosition){

    bool bCornered=false;
    float sinePart=2*M_PI;
    float circlePos=float(drawPosition)/float(listType.size());
    maxRadius=1.1*scale.y;


    if (listWidth>0)
        listButton[buttonNumber]->scale.x=listWidth;
    if (listHeight>0)
        listButton[buttonNumber]->scale.y=listHeight;

    Vector3f loc;
    switch(listDisplayMode){


        case 0:                     //to the right and down
            loc.x=location.x+scale.x+2;
            loc.y=location.y+drawPosition*(listButton[buttonNumber]->scale.y+2);
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 1:                     //weird thing for brush-size

            loc.x=location.x+(drawPosition+1)*(listButton[buttonNumber]->scale.x+2);
            loc.y=location.y-(listButton[buttonNumber]->scale.y-scale.y);
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 2:                       //circular
            if (bCornered)
             sinePart=M_PI;
            loc.x=location.x+radius*sin(circlePos*sinePart);
            loc.y=location.y+radius*cos(circlePos*sinePart);
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 3:                       //straight up
            loc.x=location.x;
            loc.y=location.y-scale.y-drawPosition*(listButton[buttonNumber]->scale.y+2);
            listButton[buttonNumber]->setLocation(loc);
            break;

        case 4:                       //box, right - used for texture browser
            //find row
            int row=drawPosition/listColumns;
            //find column
            int column = drawPosition%listColumns;
            loc.x=location.x + scale.x + column * (listButton[buttonNumber]->scale.x+2);
            loc.y=location.y + row * (listButton[buttonNumber]->scale.y+2);
            listButton[buttonNumber]->setLocation(loc);
            break;
    }
    listButton[buttonNumber]->initialLocation=listButton[buttonNumber]->location;
}

void ListButton::update(double deltaTime){

if (listDisplayMode==2 && radius<maxRadius)
  {
  radius+=animSpeed*deltaTime;
  radius=min(radius,maxRadius);

  for (unsigned int i=0;i<listType.size();i++)
     placeButton(i,i);
  }
}

void ListButton::deselect(int depth){

    if (listButton.size()>0){// && !listButton[0]->bPermanent){
        cout << "clearing list..." << endl;
        listButton.clear();
        }

    //get rid of scrollButton if we're not permanent!
    if (scrollBar){
        scrollBar->bPermanent=false;
        scrollBar->remove();
        scrollBar=NULL;
    }

    //remove myself from screen if I am not a menu item!
    //will be determined by depth
    BasicButton::deselect(depth);
}

void ListButton::trigger(Actor* other){

    if (scrollBar && other==scrollBar){
        //if sliderValue is 1, begin is size()-maxDraw
        //if sliderValue is 0, begin is 0

        if (listButton.size()==0)
            return;

        beginListItem =(int)(scrollBar->sliderValue * ((int)listButton.size() - maxListItems));
        //hide all
        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bHidden=true;
        }
        //unhide the ones in range
        //and adjust positions
        int btnPos=0;
        for (int i=beginListItem;i<(int)beginListItem+maxListItems;i++){
            listButton[i]->bHidden=false;
            placeButton(i,btnPos++);
        }
    return;
    }

    if (other->name=="exit")
        exit(0);
}

void ListButton::create(){
renderer->addButton(this);
std::cout << "creating a listButton!";
}
