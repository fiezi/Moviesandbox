uniform float screensize;
uniform float scene_size;
uniform float time;
uniform mat4 cameraInverse;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float farClip;

varying float zPos;
varying float oID;


/*
*   Main
*/

void main(){

    //texturing
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[0].x *=1.0;
    gl_FrontColor=gl_Color;

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;


    //zPos=farClip;
    oID= (objectID+100.0) /2048.0;

}
