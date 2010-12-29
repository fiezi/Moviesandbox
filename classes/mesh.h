#ifndef H_MESH
#define H_MESH

#include "includes.h"

class Mesh
{
public:
       std::vector <Vector3f> vertexData;
       void loadOBJFile(const char* filename);
};
#endif
