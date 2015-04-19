attribute float vertexID;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float farClip;
uniform float fov;

varying float zPos;
varying float oID;

/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w *(45.0)/(fov) ;
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


  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

/*
  float xC=myVertex.x;
  myVertex.x+=0.005*myVertex.y*sin(1.0*sin(0.01*myVertex.y+0.0025* time));
  myVertex.z+=0.0025*cos(xC+0.008* time);
  myVertex.y+=0.025*sin(myVertex.z+0.008* time);
*/
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize() * (1.0+0.15* sin(0.001 + objectID +myVertex.y) +0.15* sin(0.001 + objectID +myVertex.x+myVertex.z));


  gl_FrontColor.rgb= gl_Color.rgb;//* vec3(0.9+0.1* sin(0.001 + objectID +myVertex.y), 0.9+ +0.1* sin( myVertex.x), 0.9 + 0.1 * sin(myVertex.z));
  gl_FrontColor.a=1.0;
    zPos=gl_Position.z/farClip;
    oID= (objectID+100.0) /2048.0;


}




