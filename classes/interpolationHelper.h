#ifndef H_INTERPOLATIONHELPER
#define H_INTERPOLATIONHELPER

#include "actor.h"
#include "particleSystem.h"

//TODO: make subclasses of interpolators for different types - Matrix, Vector, etc...

class Renderer;

class InterpolationHelper{
    public:


    float mergeFactor;
    float mergeDirection;
    bool bMerge;

    Renderer * renderer;

    Actor * moveActor;
    Actor * targetActor;

    bool bInterpolateVectors;
    bool bInterpolateMatrix;
    bool bInterpolateProperty;
    bool bInterpolateActor;

    bool bFinishedMatrix;
    bool bFinishedVectors;

    bool bRelative;
    bool bAdditive;
    bool bLinear;
    bool bLooping;


    bool bFinished;

    vector <key*>    keyFrames;

    Vector3f    baseLocation;
    Vector3f    baseRotation;
    Matrix4f    baseMatrix;

    std::map<std::string, Matrix4f> baseBoneMatrices;

    int currentKey;
    int currentKeyMatrix;
    int currentKeyVectors;

    double currentTime;
    double startTime;

    float inPoint;

    float timeScale;

    float moveTime;



    InterpolationHelper();

    virtual ~InterpolationHelper();

    virtual void interpolate();
    virtual void interpolateVectors();
    virtual void interpolateMatrix();
    virtual void interpolateActor();

    virtual void reset();
    virtual void fadeOut(int key);

    virtual Matrix3f getRotationMatrix(Matrix4f source);
    virtual Matrix3f normalizeRotations(Matrix3f source);
};
#endif
