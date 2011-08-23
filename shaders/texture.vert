uniform float screensize;
uniform float scene_size;
uniform float time;
uniform mat4 cameraInverse;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;

varying float zPos;
varying vec4 picking;

varying vec3 N;
varying float backSide;

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

    //texturing
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor=gl_Color;

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;

    N =  gl_NormalMatrix * gl_Normal;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;
    //vec4 myPos = gl_ModelViewMatrix * myVertex;
    //myPos=myPos/myPos.w;

    gl_PointSize= pointSize() * screensize/scene_size;

    //zPos from 0 to 1, then from 0 to 65536.0!
    zPos=gl_Position.z;
    //zPos=myPos.z;

}
