uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;

uniform mat4 cameraInverse;
uniform float objectID;

varying float zPos;
varying vec4 picking;
varying vec3 N, L;

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

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize();

  N = gl_NormalMatrix *   gl_Normal;

  vec4 V = gl_ModelViewMatrix * myVertex;
  L = gl_LightSource[0].position.xyz - V.xyz;

  gl_FrontColor=clamp(gl_Color, 0.0, 1.0);

  picking= cameraInverse * gl_ModelViewMatrix * myVertex;
  picking.w = objectID;
  zPos=gl_Position.z;
}




