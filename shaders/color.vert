attribute float vertexID;
uniform float screenX;
uniform float screenY;
uniform float farClip;
uniform float nearClip;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float fov;

varying float zPos;
varying float vID;
varying float pSize;
varying vec2 coord;

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

                                                    //this here is an approximation
    gl_PointSize= pointSize();//  * (45.0*45.0)/(fov*fov);

    pSize=gl_PointSize;///screenX;

    zPos=gl_Position.z;

    zPos/=farClip;

    vID=vertexID;
    vec4 pixelPos= gl_Position;
    pixelPos /= pixelPos.w;
    pixelPos= pixelPos * 0.5 + 0.5;
    coord=pixelPos.xy;
}




