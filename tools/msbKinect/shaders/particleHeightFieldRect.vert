#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex;
//uniform sampler2D displacementTex;
//uniform sampler2D normalTex;

uniform float time;
uniform mat4 cameraInverse;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;

varying float zPos;
varying vec4 picking;

varying vec3 N;
/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w ;
  particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
  return ( (particleScale * particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );

}

/*
*   Main
*/

void main(){

    //texturing
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[0].s*=2.6;
    gl_TexCoord[0].t*=1.95;
    gl_FrontColor=gl_Color;

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;

    N =  gl_NormalMatrix * gl_Normal;

    myVertex.z=texture2DRect(tex,gl_TexCoord[0].st).r * 5.0;
    myVertex.xy*=myVertex.z;

    //myVertex.z=1.0;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

    if (myVertex.z>0.0)
        gl_PointSize= 1.0;
    else
        gl_PointSize= 0.0;

    picking =  cameraInverse * gl_ModelViewMatrix * myVertex ;
    picking.w = objectID;
    zPos=gl_Position.z;

}
