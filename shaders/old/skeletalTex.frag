varying vec3 N, L;
uniform vec4 postColor;
varying float zPos;

void main(){

  gl_FragData[0].xyz=gl_Color.xyz;
  gl_FragData[1]=vec4(zPos ,zPos , zPos,1.0);

  //distortion
}

