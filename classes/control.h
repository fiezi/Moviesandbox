#ifndef H_CONTROL
#define H_CONTROL

#include "actor.h"

#include "msbTool.h"
#include "selectTool.h"
#include "navTool.h"
#include "orbitTool.h"
#include "gridTool.h"
#include "drawTool.h"
#include "spawnTool.h"
#include "boneTool.h"
#include "skinTool.h"
#include "timelineInspector.h"

#define TOOL_SELECT            0
#define TOOL_NAV               1
#define TOOL_GRID              2
#define TOOL_SPAWN             3
#define TOOL_DRAW              4
#define TOOL_CALLIGRAPHY       5
#define TOOL_PARTICLESELECT    6
#define TOOL_BONE              7
#define TOOL_SKIN              8
#define TOOL_ORBIT             9


#define FLYINGCAMERA 0
#define WALKINGCAMERA 1

class Input;
class SceneData;
class Node;



class Control: public Actor {

public:

    MsbTool* currentTool;
    vector<MsbTool*> myTools;

    TimelineInspector* timeline;

    static bool bRunning;
    static vector <Node *> nodeList;              //all Nodes
    static vector<Node *> nodeTree;               //currently active nodes

    Actor *controlledActor;

    Vector3f lookPoint;
    Vector3f upPoint;

    Vector2f oldMousePos;

    int tool,
       oldTool;

    int moveMode;

    vector<string> eventTrigger;
    Control();

    void setup();
    void update(double deltaTime);

    void trigger(MsbObject* other);
    void startMovie();

    void switchTool(int newTool);

    void start();
    void stop();
    void updateNodes();

    void moveAutoCamera(float deltaTime);

};
#endif
