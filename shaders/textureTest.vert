uniform float screensize;
uniform float scene_size;
uniform float time;
uniform mat4 cameraInverse;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float farClip;
uniform sampler2D tex;

varying float zPos;
varying float oID;

varying vec4 VertexColor;
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

    float oneStep=1.0/20.0;
    //texturing
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[0] = gl_TextureMatrix[0] * vec4(gl_Vertex.x*0.5+0.5,gl_Vertex.y *0.5+0.5,0,0);
    gl_FrontColor=gl_Color;

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;

    gl_FrontColor=texture2D(tex,vec2(gl_Vertex.x*0.5+0.5,gl_Vertex.y *0.5+0.5));
    vec4 colorLeft=texture2D(tex,vec2(gl_Vertex.x*0.5+0.5-oneStep,gl_Vertex.y *0.5+0.5));
    vec4 colorRight=texture2D(tex,vec2(gl_Vertex.x*0.5+0.5+oneStep,gl_Vertex.y *0.5+0.5));

    vec4 colorUp=texture2D(tex,vec2(gl_Vertex.x*0.5+0.5,gl_Vertex.y *0.5+0.5+oneStep));
    vec4 colorDown=texture2D(tex,vec2(gl_Vertex.x*0.5+0.5,gl_Vertex.y *0.5+0.5-oneStep));


        myVertex.z=gl_FrontColor.a*6.0;

    float depth=gl_FrontColor.a;
    float thresh=0.1;
    gl_FrontColor=vec4(0.5,0.5,0.5,1.0);

    if ((depth-colorLeft.a)* 1.0>thresh)
        gl_FrontColor.a=0.0;

    if (depth-colorRight.a>thresh)
        gl_FrontColor.a=0.0;

    if (depth-colorUp.a>thresh)
        gl_FrontColor.a=0.0;

    if (depth-colorDown.a>thresh)
        gl_FrontColor.a=0.0;
/*
    gl_FrontColor.g=(depth-colorLeft.a)*10.0;
    gl_FrontColor.b=(depth-colorRight.a)*10.0;
    gl_FrontColor.r=(depth -colorUp.a)*10.0;
    gl_FrontColor.r+=(depth -colorDown.a)*10.0;
*/
    myVertex.x=myVertex.x*myVertex.z;// + 0.5 * sin (time   *0.001 + myVertex.y);
    myVertex.y=myVertex.y*myVertex.z* 0.35;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

    gl_PointSize= pointSize();

    zPos=gl_Position.z/farClip;
    oID= (objectID+100.0) /1024.0;

}
