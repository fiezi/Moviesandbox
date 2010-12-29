#ifndef COLLADALOADER_H_INCLUDED
#define COLLADALOADER_H_INCLUDED


#include "includes.h"
#include "renderer.h"
#include "meshData.h"

using namespace std;

class ColladaLoader{

    public:

        Renderer* renderer;
    //COLLADA can do 3 different types of geometry - polygons, polylists and triangles
    //we only do triangles here

        Vector3f* normals;                               //normal array pointer
        Vector3f* vertices;                              //vertex array pointer
        Vector3f* texCoords;

        Vector4f* vertexWeights; //4 weights per vertex, we select the 4 most influential
        Vector4f* boneReference; //4 bone References per vertex, we select the 4 most influential

        std::vector <std::string> boneNames;



        ColladaLoader();
        virtual ~ColladaLoader();

        bool loadColladaMesh ( string filename, string meshID );

        bool loadColladaBones(string meshID, TiXmlHandle hRoot);

        void recurseThroughNodes(TiXmlElement * element, bone* parentBone, string meshID);

        bool loadBoneNames(string meshID, TiXmlElement * sourceElement);
        bool loadBindShapeMatrix(string meshID, TiXmlElement * sourceElement);
        bool loadInvBoneMatrices(string meshID, TiXmlElement * sourceElement);
        bool loadPerVertexWeights(string meshID, TiXmlElement * sourceElement);

        bool loadColladaVertices(string meshID, TiXmlElement * sourceElement);
        bool loadColladaNormals(string meshID, TiXmlElement * sourceElement);
        bool loadColladaTexCoords(string meshID, TiXmlElement* sourceElement);

        bool createVBOs(string meshID, TiXmlElement* sourceElement);

};

#endif // COLLADALOADER_H_INCLUDED
