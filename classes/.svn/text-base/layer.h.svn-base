#ifndef H_LAYER
#define H_LAYER

#include "basicButton.h"

class Layer: public BasicButton{

public:

    std::string depthTextureID;
    std::string pickTextureID;
    std::string lightDataTextureID;

    GLuint depthFBO;
    GLuint depthTex;

    GLuint colorFBO;
    GLuint colorTex;

    GLuint pickFBO;
    GLuint pickTex;

    GLuint lightDataFBO;
    GLuint lightDataTex;

    std::vector<Actor*> actorList;


    Layer();
    virtual ~Layer();

    virtual void setup();
    virtual void update(double deltaTime);
    virtual void updateShaders();

    virtual void mouseOver();

    virtual void mouseDrag();
    virtual void finishDrag();

    virtual void clickedLeft();
    virtual void clickedRight();

    virtual void focusClick();

    virtual void deselect(int depth);

    virtual void create();
};
#endif // LAYER
