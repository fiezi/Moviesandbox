#ifndef H_LISTBUTTON
#define H_LISTBUTTON

#include "basicButton.h"
#include "sliderButton.h"

class ListButton: public BasicButton{

public:

    int listDisplayMode;                            //mode 0:
                                                    //mode 1:
                                                    //mode 2:

    float radius;                                   //for radial menu
    float maxRadius;                                //for radial menu
    float animSpeed;                                //for radial menu
    float listWidth;                                //width of every button in the list
    float listHeight;                               //height of every button in the list

    int listColumns;

    std::vector <std::string> listType;            //registers the types of Buttons you want the list to populate
    std::vector <std::string> listProp;            //the buttonProperty for each Button in the List
    std::vector <std::string> listName;            //the name for each Button in the List
    std::vector <std::string> listIcon;            //the icon for each Button in the List

    std::vector <BasicButton *> listButton;             //holds the actual references to the Buttons - will be populated by assembleList();

    SliderButton* scrollBar;                      //holds the scroll button;

    int maxListItems;                              //max amount of listButtons to show
    int beginListItem;                             //where to start displaying from

    bool bPermanentList;                           //make listButtons permanent = undeletable
                                                   //you must manually clean them up then!

    Vector4f listColor;                            //color of the buttons in the list
    string listShader;                             //shader to use (usually color or texture)

    ListButton();
    virtual ~ListButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void clickedLeft();                    //calls assembleList() and placeButton()

    virtual void assembleList();                                    //creates Buttons from the listType vector and puts them in listButton vector as Button references
    virtual void placeButton(int buttonNumber, int drawPosition);   //sets location of listButtons
    virtual void createScrollBar();                                 //create slider to scroll through listButtons
    virtual void placeScrollBar();                                  //where it should appear

    virtual void trigger(Actor* other);                             //callback from Scrollbar

    virtual void deselect(int depth);


    virtual void create();
};
#endif
