#ifndef H_INPUTCONNECTBUTTON
#define H_INPUTCONNECTBUTTON

#include "basicButton.h"

//this class is used to store the connection properties for every dataset received by a udpInput Button
//stored in Actor* connectedTo and string buttonProperty
//Actor* parent has connection to the udpInput
class InputConnectButton: public BasicButton{

public:

    InputConnectButton();

    virtual ~InputConnectButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void mouseOver();
    virtual void mouseDrag();
    virtual void clickedLeft();
    virtual void clickedRight();
    virtual void focusClick();
    virtual void deselect(int depth);
    virtual void create();

    virtual void trigger(Actor * other);

    //Position in the inputConnectButtons list of UDPInput
    int     listPosition;
    Actor*  connectedActor;
};
#endif // INPUTCONNECTBUTTON
