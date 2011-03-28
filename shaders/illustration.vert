attribute float vertexID;
uniform float screensize;
uniform float scene_size;

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
  float xC=myVertex.x;
  myVertex.x+=0.00025*myVertex.y*sin(myVertex.y+0.01* time);
  myVertex.z+=0.00025*cos(xC+0.008* time);
  myVertex.y+=0.00025*sin(myVertex.z+0.008* time);

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize() * (1.15+0.15* sin(0.001* time * objectID * xC +xC)) * screensize/scene_size;
  smudge=smudge*(gl_PointSize/30.0)*(gl_PointSize/10.0)*(gl_PointSize/2.0);

  N=gl_NormalMatrix * gl_Normal;
  picking= cameraInverse * gl_ModelViewMatrix * myVertex;
  picking.w = objectID;

  zPos=gl_Position.z;

  vID=vertexID;
}




