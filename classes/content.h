#ifndef H_CONTENT
#define H_CONTENT

#include "msbObject.h"
#include "basicButton.h"

class ParticleSystem;
class Renderer;

class Content: public MsbObject{
public:

    virtual void setup();
    virtual void reset();

    void createMenuBar();

    void createMonitors();

    void createConsole();

    void createSelectButton(int x, int y);

    void createNavButton(int x, int y);

    void createGridButton(int x, int y);

    void createPropList(int x, int y);

    void createDrawWidget(int x, int y);

    void createBoneWidget(int x, int y);

    void createNodeList(int x, int y);

    void createSysMenu(int x, int y);

    void createInspectors();

    void createActorContent();

};
#endif
