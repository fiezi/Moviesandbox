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

/*
float pointSize(float p){

  float particleScale=  p *  gl_Position.w ;
  //particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  //particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
  return ( (particleScale * particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );

}
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w ;
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

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    //float pS= myVertex.w;
    myVertex.w=1.0;

    N =  gl_NormalMatrix * gl_Normal;

    gl_FrontColor=texture2D(tex,gl_TexCoord[0].st);
    myVertex.z=gl_FrontColor.a*8.0;
    myVertex.x=myVertex.x*myVertex.z;// + 0.5 * sin (time   *0.001 + myVertex.y);
    myVertex.y=myVertex.y*myVertex.z;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

    gl_PointSize= pointSize();
    zPos=gl_Position.z;

}
