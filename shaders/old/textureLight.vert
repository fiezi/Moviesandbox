varying vec3 N, L;
vec3 lightPos;
uniform float time;
varying float zPos;


uniform float time;
uniform mat4 cameraInverse;
uniform float objectID;

varying float zPos;
varying vec4 picking;
varying vec3 normal;

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
    gl_FrontColor=gl_Color;

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;

    normal =  gl_NormalMatrix * gl_Normal;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

    gl_PointSize= pointSize(gl_Position)*particleScale;

    vec4 V = gl_ModelViewMatrix * gl_Vertex;
    L = gl_LightSource[0].position.xyz - V.xyz;

    N = gl_NormalMatrix * gl_Normal;

    picking =  cameraInverse * gl_ModelViewMatrix * myVertex ;
    picking.w = objectID;
    zPos=gl_Position.z;

}
