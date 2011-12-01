uniform float time;

float pointSize(vec4 loc)
{
float ptSize = length(loc);
return (10000.0/ptSize);
}


//changed so that sprites can also use this shader
void main()
{
//reset gl_Vertex coordinate or we create weird distortions!
vec4 myVertex=gl_Vertex;
myVertex.w=1.0;
float particleScale=1.0;
vec4 intermediate= gl_ModelViewMatrix * myVertex;
gl_Position = gl_ProjectionMatrix * intermediate;

particleScale=gl_Vertex.w * 1.0; // + abs(sin(cos(time*0.002)*0.08 *gl_Vertex.x))*0.8;

  int step=1;
  
  if (length(gl_Position)>23.0){
    step=2;
    particleScale=particleScale *1.5;
  }
  
  if (length(gl_Position)>35.0){
    step=4;
    particleScale=particleScale * 1.8 ;
  }

  if (length(gl_Position)>65.0){
    step=16;
    particleScale=particleScale * 2.8;
  }
  
  if (length(gl_Position)>85.0){
    step=32;
    particleScale=particleScale*1.5;
  }

  if (length(gl_Position)>100.0){
    step=64;
    particleScale=particleScale*1.5;
  }

    
    if ( length(gl_Position)>15.0 &&( int(gl_Vertex.x * 400.0)%step >0 || int(gl_Vertex.y * 400.0)%step >0 )){
      gl_FrontColor=gl_Color;
      //gl_Position=vec4(0.0);
      gl_PointSize= 1.0;
      }
    else{
      gl_FrontColor=gl_Color;
      gl_PointSize= pointSize(intermediate)*particleScale;
      }

gl_FrontColor.a=1.0;
}




