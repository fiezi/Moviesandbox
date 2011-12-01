varying float zPos;
varying float yPos;

void main()
{
zPos=gl_Vertex.z;
yPos=gl_Vertex.y;
gl_Position = ftransform();
}
