varying float zPos;

void main()
{
  gl_FragColor=vec4(zPos);
  gl_FragColor.a = 1.0;
}

