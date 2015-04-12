attribute float vertexID;
uniform float screenX;
uniform float screenY;
uniform float farClip;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float fov;

varying float zPos;
varying float oID;


/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w *(45.0)/(fov) ;
  //return particleScale;
  //particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  //particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
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

    /*
    //Ocean vertices
    myVertex.z+=1.0*sin(30.0*myVertex.x+time*0.0015);
    myVertex.x+=0.02*sin(1.0*myVertex.y*20.0+time*0.001);
    myVertex.y+=0.1*sin(5.0*myVertex.x*2.0+time*0.0002);
    myVertex.z+=0.5*sin(20.0*myVertex.y+time*0.0005);
    */


    //myVertex.z+=0.5*sin(10.0*myVertex.x+time*0.0015);

    //myVertex.x+=1.0*sin(1.0*20.0+time*0.001);
    //myVertex.y+=1.0*sin(2.0+time*0.0002);

    //myVertex.z+=0.05*sin(20.0*myVertex.y+time*0.0005);

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

                                                    //this here is an approximation
    gl_PointSize= pointSize();

    //testing for ocean


    zPos=gl_Position.z/farClip;
    oID= (objectID+100.0) /2048.0;

}




