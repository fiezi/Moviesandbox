#include "mesh.h"

void Mesh::loadOBJFile(const char* filename)
{
FILE *fp = NULL;
char buffer[256];

fp = fopen(filename,"r");
assert(fp);

Vector3f vertex;

while(!feof(fp))
  {
  memset(buffer,0,255);
  /*	Grab a line at a time	*/
  fgets(buffer,256,fp);
  //	look for the 'v ' - vertex co-ordinate - flag

  if( strncmp("v ",buffer,2) == 0 )
    {
    sscanf((buffer+1),"%f%f%f",
    &vertex.x,&vertex.y,&vertex.z);
    vertexData.push_back(vertex);
    //cout <<buffer;
    }
  }
  fclose(fp);
}
