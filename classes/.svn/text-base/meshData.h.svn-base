#ifndef H_MESHDATA
#define H_MESHDATA

#include "actor.h"
#include "renderer.h"


class MeshData: public Actor{

public:



    std::vector<int>    vertexCount;
    std::vector<GLuint> vertexIDObject;
    std::vector<GLuint> vertexBufferObject;
    std::vector<GLuint> normalBufferObject;
    std::vector<GLuint> colorBufferObject;
    std::vector<GLuint> secondaryColorBufferObject;
    std::vector<GLuint> texCoordBufferObject;

    std::vector<GLuint> vertexWeightsObject; //Vertex Bone Weights 1-4
    std::vector<GLuint> boneReferenceObject; //Bone References

    std::vector<vertexData> vData;

    bool bIsSkeletal;
    bool bIsHead;
    bool bVertexColor;
    bool bEditable;

    int boneCount;
    int texCoordPerVertexCount;
    int verticesPerShapeCount;              //TODO: should be "numbers per Vertex count" - 3 or 4
    GLenum  vertexInterpretation;
    std::vector<bone*>   bones;
    Matrix4f* bindShapeMatrix;              //points to an array of bone matrices (according to number of bones in boneCount)

    MeshData();
    virtual ~MeshData();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(Actor * other);

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // MESHDATA
