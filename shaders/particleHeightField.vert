uniform sampler2D tex;
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

float pointSize(float p){

  float particleScale=  p *  gl_Position.w ;
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

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;

    N =  gl_NormalMatrix * gl_Normal;

    gl_FrontColor=texture2D(tex,gl_TexCoord[0].st);
    myVertex.z=gl_FrontColor.a*10.0;
    myVertex.x=myVertex.x*myVertex.z;
    myVertex.y=myVertex.y*myVertex.z;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

    gl_PointSize= 5.0;//pointSize(1.0);

    picking =  cameraInverse * gl_ModelViewMatrix * myVertex ;
    picking.w = objectID;
    zPos=gl_Position.z;

}
