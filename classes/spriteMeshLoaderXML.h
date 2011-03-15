#ifndef SPRITEMESHLOADERXML_H_INCLUDED
#define SPRITEMESHLOADERXML_H_INCLUDED


#include "msbObject.h"
#include "skeletalActor.h"

using namespace std;


class SpriteMeshLoaderXML: public MsbObject{

    public:

        Vector4f* vertices;
        Vector4f* colors;
        Vector3f* secondaryColors;
        Vector3f* normals;
        Vector3f* texCoords;

        Vector4f* vertexWeights; //4 weights per vertex, we select the 4 most influential
        Vector4f* boneReference; //4 bone References per vertex, we select the 4 most influential

        int vertexCount;

        SpriteMeshLoaderXML();
        virtual ~SpriteMeshLoaderXML();

        bool saveSpriteMesh ( string filename, SkeletalActor* myDrawing );

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
