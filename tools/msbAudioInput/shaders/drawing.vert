attribute float vertexID;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform mat4 cameraInverse;
uniform float objectID;
uniform vec4 lightLoc;

varying float zPos;
varying vec4 picking;
varying vec3 N;
varying vec3 smudge;
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
  return ( (particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );

}

/*
*   Main
*/

void main(){

  gl_FrontColor=gl_Color;

  smudge=gl_NormalMatrix * gl_SecondaryColor.rgb;
  normalize(smudge);

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize();
  pSize=gl_PointSize;


//give normal as up vector!

	if (gl_Normal!=gl_Normal * 0.0){
		N = gl_NormalMatrix * gl_Normal;
		bTubeNormal=0.0;
		}
	else{
		N = gl_NormalMatrix * vec3(0.0,1.0,0.0);
		bTubeNormal=100.0;
		}
  picking= cameraInverse * gl_ModelViewMatrix * myVertex;
  picking.w = objectID;

  zPos=gl_Position.z;



//code to generate gl_FragCoord from gl_Position
//Useful for texturing large points


  pixelPos= gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
  pixelPos /= pixelPos.w;
  pixelPos= pixelPos * 0.5 + 0.5;


  vID=vertexID;
}




