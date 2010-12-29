#ifndef H_ROTATEBUTTON
#define H_ROTATEBUTTON
#include "basicButton.h"

class RotateButton: public BasicButton
{
public:
        bool bActive;
        vector<Vector3f> fineRotation;
        vector<Vector3f> fineLocation;

        RotateButton();
        virtual ~RotateButton();

        virtual void update(double deltaTime);

        virtual void clickedLeft();
        virtual void clickedRight();
        virtual void focusClick();
        virtual void create();
};
#endif
