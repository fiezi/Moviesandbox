varying vec3 N, L;
uniform vec4 postColor;
varying float zPos;

void main()
{

//lighting
	const float specularExp = 64.0;
	vec3 NN = normalize(N);
	vec3 lightCol;
	lightCol = vec3(1.0,1.0,1.0);
	
//ambient Light
  gl_FragData[0]=0.4 * gl_Color;

	vec3 NL = normalize( L );
	vec3 NH = normalize( NL +vec3(0.0,0.0,1.0) );
	float NdotL = max(0.0,dot(NN,NL));
	gl_FragData[0].rgb += 1.0  * gl_Color.rgb *lightCol *NdotL;
	
  gl_FragData[0].rgb=clamp(gl_FragData[0].rgb, 0.0, gl_Color.rgb);

//specular HighLight
	if( NdotL > 0.0 )
		gl_FragData[0].rgb += 0.4 * lightCol * pow(max(0.0,dot(NN,NH)),specularExp);
/*  
//toon shading
	if (NdotL > 0.25)
		gl_FragData[0] = 1.0 * gl_Color;
	else 
		gl_FragData[0] = 0.55 * gl_Color;


*/


//post processing - color multiplier
  gl_FragData[0]=vec4(postColor.r * gl_FragData[0].r, postColor.g * gl_FragData[0].g, postColor.b * gl_FragData[0].b, 1.0);

//post processing - saturation
  vec4 greyColor= vec4( (gl_FragData[0].r+gl_FragData[0].g+gl_FragData[0].b)/3);
  
  greyColor.b+=0.025;
  greyColor.a=1.0;
  gl_FragData[0]= (postColor.a * gl_FragData[0] + (1-postColor.a) * greyColor);

  gl_FragData[0].a = 1.0;
  
  gl_FragData[1]=vec4(zPos ,zPos , zPos,1.0);
  
}



