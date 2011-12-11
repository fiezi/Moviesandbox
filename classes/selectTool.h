#ifndef H_SELECTTOOL
#define H_SELECTTOOL

#include "msbTool.h"

class SelectTool: public MsbTool{

public:

    TiXmlElement*   clipboard;

    bool bKeepSelection;
    bool bClickedonGizmo;

    SelectTool();
    virtual ~SelectTool();

    virtual void setup();

    virtual void start();
    virtual void stop();

    virtual void keyPressed(int key);
    virtual void keyReleased(int key);

    virtual void mousePressed(int btn);
    virtual void mouseReleased(int btn);
    virtual void mouseDragged(int btn);

    virtual void update(double deltaTime);
    virtual void selectActors(int btn, Actor* other);
    virtual void makeGroup();
    virtual void makePrefab(std::string prefabName);

    virtual void pasteSelected();
    virtual void cutSelected();
    virtual void copySelected();
    virtual void duplicateSelected();
};
#endif // SELECTTOOL
