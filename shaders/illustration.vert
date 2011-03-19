attribute float vertexID;
uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform mat4 cameraInverse;
uniform float objectID;
varying float zPos;
varying vec4 picking;
varying vec3 N;
varying float vID;
varying vec3 smudge;

/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w ;
  particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
  if (gl_Position.z>0.3){
      if (gl_Position.z<1.0 )
        return ( (particleScale * 1000.0  ) / (gl_Position.z) );

      return ( (particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );
  }else
  return 1.0;

}

/*
*   Main
*/

void main(){

  gl_FrontColor=gl_Color;

  smudge=gl_NormalMatrix * gl_SecondaryColor.rgb;
  //normalize(smudge);
  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

  myVertex.x+=0.0025*myVertex.y*sin(myVertex.y+0.005* time);
  myVertex.z+=0.0025*sin(myVertex.x+0.0004* time);
  myVertex.y+=0.00025*sin(myVertex.z+0.02* time);

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize() * (1.0+0.05* sin(myVertex.x * 0.004* time));
  smudge=smudge*(gl_PointSize/20.0)*(gl_PointSize/10.0)*(gl_PointSize/2.0);

  N=gl_NormalMatrix * gl_Normal;
  picking= cameraInverse * gl_ModelViewMatrix * myVertex;
  picking.w = objectID;

  zPos=gl_Position.z;

  vID=vertexID;
}




