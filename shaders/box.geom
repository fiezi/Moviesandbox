#version 330
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;

in float farClip[1];
in float pScale[1];
in float VoID[1];
in mat4 VtransMat[1];

out float zPos;
out float oID;
layout(triangle_strip, max_vertices=20) out;

void goVertex(float x, float y, float z){

    vec4 myPos=gl_in[0].gl_Position;
    float size=myPos.w * pScale[0];
    myPos.w=1.0;
    gl_Position = VtransMat[0] *(myPos+vec4(x*size,y*size,z*size,0));
    zPos=(gl_Position.z+z*size)/512.0;
    oID=VoID[0];
    EmitVertex();
}

void main()
{
    gl_FrontColor= gl_FrontColorIn[0];

    //back
    goVertex(-0.5,-0.5,-0.5);
    goVertex(-0.5,0.5,-0.5);
    goVertex(0.5,-0.5,-0.5);
    goVertex(0.5,0.5,-0.5);

    EndPrimitive();

    //front
    goVertex(-0.5,-0.5,0.5);
    goVertex(-0.5,0.5,0.5);
    goVertex(0.5,-0.5,0.5);
    goVertex(0.5,0.5,0.5);

    EndPrimitive();

    //left
    goVertex(0.5,-0.5,-0.5);
    goVertex(0.5,0.5,-0.5);
    goVertex(0.5,-0.5,0.5);
    goVertex(0.5,0.5,0.5);

    EndPrimitive();

    //right
    goVertex(-0.5,-0.5,-0.5);
    goVertex(-0.5,0.5,-0.5);
    goVertex(-0.5,-0.5,0.5);
    goVertex(-0.5,0.5,0.5);

    EndPrimitive();

    //top
    goVertex(-0.5,0.5,0.5);
    goVertex(-0.5,0.5,-0.5);
    goVertex(0.5,0.5,0.5);
    goVertex(0.5,0.5,-0.5);

    EndPrimitive();

}
