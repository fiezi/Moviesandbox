#ifndef H_MESHDATA
#define H_MESHDATA

#include "actor.h"

class MeshData: public Actor{

public:



    std::vector<int>    vertexCount;                    //how many vertices does this mesh have
    std::vector<GLuint> vertexIDObject;
    std::vector<GLuint> vertexBufferObject;
    std::vector<GLuint> normalBufferObject;
    std::vector<GLuint> colorBufferObject;
    std::vector<GLuint> secondaryColorBufferObject;
    std::vector<GLuint> texCoordBufferObject;

    std::vector<GLuint> vertexWeightsObject; //Vertex Bone Weights 1-4
    std::vector<GLuint> boneReferenceObject; //Bone References

    std::vector<vertexData> vData;

    bool bIsSkeletal;                       //has bones and stuff
    bool bIsHead;                           // to be implemented someday?
    bool bVertexColor;                      //uses vertex color object - COLLADA doesn't use this usually
    bool bEditable;                         //can be changed within MSB - COLLADA cannot be changed for now

    int boneCount;                          //how many bones
    int texCoordPerVertexCount;             //how many texture coordinaes per vertex?
    int verticesPerShapeCount;              //TODO: should be "numbers per Vertex count" - 3 or 4
    GLenum  vertexInterpretation;           //vertices interpreted as QUADS or TRIANGLES
    std::vector<bone*>   bones;
    Matrix4f* bindShapeMatrix;              //points to an array of bone matrices (according to number of bones in boneCount)

    //bounding box related
    Vector3f    lowerLeftBack,                  //point starts lower left hand corner (as seen from user)
                upperRightFront,
                center;                     //center of bounding box!


    MeshData();
    virtual ~MeshData();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // MESHDATA
