uniform sampler2D tex;
varying float zPos;

void main()
{
  gl_FragColor=vec4(zPos ,zPos , zPos,1.0);
//texturing
  vec4 color = texture2D(tex,gl_TexCoord[0].st);
  if (color.a<0.1){
  gl_FragColor=vec4(65536.0);
  }
//depth Test  
  if (color.a < 0.1){
    gl_FragDepth=1.0;
    }
  else{
    gl_FragDepth=gl_FragCoord.z;
    }  
}


