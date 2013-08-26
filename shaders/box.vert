attribute float vertexID;
uniform float screenX;
uniform float screenY;
uniform float farClip;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float fov;

varying float VoID;
varying mat4  VtransMat;
varying float pScale;


/*
*   Main
*/

void main(){

    gl_FrontColor=gl_Color;
    gl_Position = gl_Vertex;
    VtransMat=gl_ModelViewProjectionMatrix;
    pScale=particleMultiplier;

                                                    //this here is an approximation
    VoID= (objectID+100.0) /1024.0;
}




