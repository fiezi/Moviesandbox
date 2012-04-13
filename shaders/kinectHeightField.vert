uniform sampler2D tex;

uniform float farClip;
uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;

varying float zPos;
varying float oID;

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

    gl_FrontColor=texture2D(tex,gl_TexCoord[0].st);
    myVertex.z=gl_FrontColor.a*8.0;
    myVertex.x=myVertex.x*myVertex.z;// + 0.5 * sin (time   *0.001 + myVertex.y);
    myVertex.y=myVertex.y*myVertex.z* 0.35;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

    if (gl_FrontColor.a>0.99)
        gl_PointSize= 0.0;
    else
        gl_PointSize= pointSize();

    zPos=gl_Position.z/farClip;
    oID= (objectID+100.0) /1024.0;

}
