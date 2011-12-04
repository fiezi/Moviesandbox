attribute float vertexID;



/*
*   Main
*/

void main(){

  gl_FrontColor=gl_Color;

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

}




