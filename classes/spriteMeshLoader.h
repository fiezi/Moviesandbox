#ifndef SPRITEMESHLOADER_H_INCLUDED
#define SPRITEMESHLOADER_H_INCLUDED


#include "msbObject.h"
#include "meshData.h"

using namespace std;


class SpriteMeshLoader: public MsbObject{

    public:


        float*  vertexIDs;                              //IDs for every single vertex

        Vector4f* vertices;                             //vertex array pointer
        Vector4f* colors;                               //color pointer
        Vector3f* secondaryColors;                      //secondary color pointer
        Vector3f* normals;                              //normal array pointer
        Vector3f* texCoords;

        Vector4f* vertexWeights; //4 weights per vertex, we select the 4 most influential
        Vector4f* boneReference; //4 bone References per vertex, we select the 4 most influential

        int vertexCount;

        SpriteMeshLoader();
        virtual ~SpriteMeshLoader();

        bool saveSpriteMesh ( string filename, SkeletalActor* myDrawing );

        void saveVertices(TiXmlElement* root);
        void saveNormals(TiXmlElement* root);
        void saveTexCoords(TiXmlElement* root);
        void saveColors(TiXmlElement* root);
        void saveSecondaryColors(TiXmlElement* root);
        void saveBoneReferences(TiXmlElement* root);
        void saveVertexWeights(TiXmlElement* root);
        void saveBones(TiXmlElement* root, SkeletalActor* myDrawing);

        bool loadSpriteMesh ( string filename, string meshID );

        void loadVertices ( string meshID, TiXmlElement * sourceElement );
        void loadNormals ( string meshID, TiXmlElement * sourceElement );
        void loadTexCoords(string meshID, TiXmlElement * sourceElement);
        void loadColors(string meshID, TiXmlElement * sourceElement);
        void loadSecondaryColors(string meshID, TiXmlElement * sourceElement);
        void loadBoneReferences ( string meshID, TiXmlElement * sourceElement );
        void loadVertexWeights ( string meshID, TiXmlElement * sourceElement );
        void loadBones ( string meshID, TiXmlElement * sourceElement );

        bool createVBOs(string meshID);

};

#endif // SPRITEMESHLOADER_H_INCLUDED
