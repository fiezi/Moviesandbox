#include "tabbedListButton.h"
#include "renderer.h"
#include "input.h"

TabbedListButton::TabbedListButton(){


    currentTab=0;
    tabColor=Vector4f(0.8,0.8,0.8,1.0);
    selectedTabColor=Vector4f(0.4,0.4,0.4,1.0);
    tabs.push_back( new Tab(this) );

}

TabbedListButton::~TabbedListButton(){}



void TabbedListButton::Tab::assembleList() {

    mine->assembleList();

}


void TabbedListButton::setup(){

    ListButton::setup();

}

void TabbedListButton::clickedLeft(){

  BasicButton::clickedLeft();

  //destroy all buttons that have higher depth than mine!
  input->deselectButtons(level);

  if (listButton.size()==0){
    cout << "clicked! creating buttonlist for: " << currentTab << endl;
    tabs[currentTab]->assembleList();
  }

}

void TabbedListButton::assembleList(){

}

void TabbedListButton::update(double deltaTime){

    ListButton::update(deltaTime);
}

void TabbedListButton::deselect(int depth){

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

void TabbedListButton::trigger(MsbObject* other){

    ListButton::trigger(other);
    tabs[currentTab]->trigger(other);

    for (int i=0;i<(int)tabTriggerButtons.size();i++)
        tabTriggerButtons[i]->color=tabColor;

    tabTriggerButtons[currentTab]->color=selectedTabColor;

}

void TabbedListButton::create(){
sceneData->addButton(this);
std::cout << "creating a listButton!";
}
