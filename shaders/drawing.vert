attribute float vertexID;

uniform float screensize;
uniform float scene_size;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;

varying float zPos;
varying vec4 pixelPos;
varying float pSize;
varying float bTubeNormal;
varying float vID;

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

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize() * screensize/scene_size;


  pSize=gl_PointSize;

  zPos=gl_Position.z;



//code to generate gl_FragCoord from gl_Position
//Useful for texturing large points


  pixelPos= gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
  pixelPos /= pixelPos.w;
  pixelPos= pixelPos * 0.5 + 0.5;


  vID=vertexID;
}




