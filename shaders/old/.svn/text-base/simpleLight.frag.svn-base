uniform vec4 postColor;

varying vec3 N, L;
varying float zPos;
varying vec4 picking;

/*
*   compute Light
*/

vec4 computeLight(vec4 ambientColor){

	const float specularExp = 256.0;
	vec3 NN = normalize(N);
	vec3 lightCol;
	lightCol = vec3(1.0,1.0,1.0);

    //ambient
    vec4 colorLight=0.0 * ambientColor;

    //diffuse
	vec3 NL = normalize( L );
	float NdotL = max(0.0,dot(NL,NN));
	colorLight.rgb += 1.0  * gl_Color.rgb * lightCol * NdotL;
    colorLight.rgb=clamp(colorLight.rgb, vec3(0.0,0.0,0.0), gl_Color.rgb);

    //specular
	if( NdotL > 0.0 ){
		vec3 NH = normalize( NL +vec3(0.0,0.0,1.0) );
        colorLight.rgb += 0.0 * lightCol * pow(max(0.0,dot(NN,NH)),specularExp);
	}


    //post processing - color multiplier
    gl_FragData[0]=vec4(postColor.r * colorLight.r, postColor.g * colorLight.g, postColor.b * colorLight.b, 1.0);

    //post processing - saturation
    vec4 greyColor= vec4( (colorLight.r+colorLight.g+colorLight.b)/3.0);

    greyColor.b+=0.025;
    greyColor.a=1.0;
    colorLight= (postColor.a * colorLight + (1.0-postColor.a) * greyColor);

    //debug
    //colorLight.xyz=NN;
    //colorLight=vec4(NdotL,NdotL,NdotL,1.0);
    //colorLight.xyz=NL;

    //write
    colorLight.a = 1.0;
    return colorLight;

}

/*
*   Main
*/

void main(){

    gl_FragData[0]=computeLight(gl_Color);
    gl_FragData[1]=vec4(N.x ,N.y , N.z,zPos);
    gl_FragData[2]=picking;
}


