uniform float time;
uniform float screensize;
uniform float shadow_size;
uniform float screenX;
uniform float screenY;

uniform float nearClip;
uniform float farClip;

uniform sampler2D tex; // rendered scene 512*512 (lighting resolution)
uniform sampler2D depthTex; // rendered normals and depth texture
uniform sampler2D shadowTex; // rendered shadow textures - 256x256 (shadow resolution)

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 cameraMatrix;
uniform mat4 cameraInverse;


uniform vec3 camLoc;
uniform vec3 camX;
uniform vec3 camY;
uniform vec3 camZ;


varying vec2 texCoord;

//light position stuff

varying vec3 lightColor;
varying vec4 lightPos;
varying float lightZScreen;
varying mat4 lightSpaceMat;

const float specularExp = 160.0;

//pixel position stuff
vec4 pixelPos;
float zPos;
float zPosScreen;


// blur variables


    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;


vec4 blur3(sampler2D myTex, vec2 tc){

      vec4 sample[9];

      float spread=1.0/screenX;//   * texture2D(myTex , tc).a/32.0;
      //float spread=0.250/shadow_size;//   * texture2D(myTex , tc).a/32.0;

      tc_offset[0]=spread * vec2(-1.0,-1.0);
      tc_offset[1]=spread * vec2(0.0,-1.0);
      tc_offset[2]=spread * vec2(1.0,-1.0);

      tc_offset[3]=spread * vec2(-1.0,0.0);
      tc_offset[4]=spread * vec2(0.0,0.0);
      tc_offset[5]=spread * vec2(1.0,0.0);

      tc_offset[6]=spread * vec2(-1.0,-1.0);
      tc_offset[7]=spread * vec2(0.0,-1.0);
      tc_offset[8]=spread * vec2(1.0,-1.0);

      for (int i=0 ; i<9 ; i++)
      {
        sample[i]=texture2D(myTex , tc + tc_offset[i]);
      }

      vec4 blurredColor=(
                         sample[0] + (2.0* sample[1]) + sample[2] +
                         (2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
                         sample[6] + (2.0* sample[7]) + sample[8]
                        )/ 13.0;
      //blurredColor.a=1.0;
      return(blurredColor);
}



float unpackToFloat(vec4 value){

	const vec4 bitSh = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

float unpackToFloat(vec3 value){

	const vec3 bitSh = vec3(1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

float unpackToFloat(vec2 value){

	const vec2 bitSh = vec2(1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}


///pixelPosition in eyeSpace ( pixel on ground plane stays the same independent of camera's distance to ground plane )
void getPixelLoc(){

    //zPos = texture2D(depthTex,texCoord).r;
    zPos= unpackToFloat(blur3(depthTex,texCoord).rg) * 512.0;
    //zPos = blur3(depthTex,texCoord ).r * 255.0 + blur3(depthTex,texCoord ).g;
    zPosScreen=farClip/ (farClip - zPos * (farClip- nearClip));
    //pixel in screen space
    pixelPos=vec4((texCoord.x-0.5) *1.1, (texCoord.y-0.5) * 0.835, zPos,1.0) ;
    //pixelPos=(vec4(texCoord.x-0.5, texCoord.y-0.5, zPos,1.0)/zPosScreen)/10.0;
    pixelPos.w=1.0;
}



vec4 computeLight(){

    //add all previous lighting calculations (from other lights) here:
    vec4 colorLight=gl_LightSource[0].ambient*texture2D(tex, texCoord);


    vec4 pp=pixelPos*(-zPos/10.0);
    //pp.z/=zPos;


    float xDist= (lightPos.x/10.0 - pp.x); //* screenX/screenY;
    float yDist= (lightPos.y/10.0 - pp.y);//* screenY/screenX;
    float zDist= (lightPos.z /10.0- pp.z)/100.0;



    vec3 lightDirection=vec3(-xDist,-yDist,zDist/1.0);
    float lightDistance=length(lightDirection)*10.0;

    vec3 lightDirectionNormalized =normalize(lightDirection);

    //return here if we're out of range!
    if (lightDistance>gl_LightSource[0].linearAttenuation)
       return vec4(0.0,0.0,0.0,1.0);

    //normal in Eye Space is the difference in z
    //we exagerrate along z - because differences become more significant with higher z
    //float dy= dFdy(zPos)/(zPos*zPos);
    float dy= dFdy(zPos);
    float dx= dFdx(zPos);
    //return vec4 (dx,dy,0.0,1.0) * 10.0;
    //return vec4 (dy) * 10.0;

    vec3 pixelNormal=normalize(vec3(dx,dy,fwidth(zPos)));

    //diffuse is dot Product of lightdirection on pixel normal
	float lightDotPixel = max(0.0,(dot(pixelNormal,(lightDirectionNormalized)) )  );
	colorLight.rgb += 1.0 * lightColor * lightDotPixel;

    //specular is exaggeration of tight angles

	if( lightDotPixel > 0.0 ){
		vec3 NH = normalize( lightDirectionNormalized +vec3(0.0,0.0,1.0) );
        colorLight.rgb += 1.0 * lightColor * pow(max(0.0,dot(pixelNormal,NH)),specularExp);
	}

    //falloff
    float falloff = max(0.0,(gl_LightSource[0].linearAttenuation-lightDistance)/gl_LightSource[0].linearAttenuation);
    colorLight= colorLight * falloff;


    return colorLight;
}



vec4 shadowMapping(){


    // return(vec4(zPos));
    vec4 myLight=vec4(0.0,0.0,0.0,0.0);

   if (gl_LightSource[0].spotCutoff==0.0){
        myLight+=computeLight( );
        return myLight;
    }


    //where do these numbers come from? and what do they want from us?

    //for 1280 * 720
    //vec4 pixelPosition=vec4((texCoord.x-0.5)*1.45, (texCoord.y-0.5)* 0.835, (-zPos) * 1.0, 1.0 )  ;

    //for 1024 * 768
    //vec4 pixelPosition=vec4((texCoord.x-0.5) *1.1, (texCoord.y-0.5) * 0.835, (-zPos) * 1.0, 1.0 )  ;
    vec4 pixelPosition=vec4((texCoord.x-0.5) *1.1, (texCoord.y-0.5) * 0.835, (-zPos) * 1.0, 1.0 )  ;
    pixelPosition.xy*=zPos;


    //Matrix transform to light space - our pixel. Matrices are computed once and can be done in CPU or vertex shader
    vec4 shadowCoord =   lightSpaceMat*pixelPosition ;
   vec2 ssShadow=shadowCoord.xy/shadowCoord.w;


        ssShadow=(ssShadow* 0.5)   + 0.5;

    //ssShadow.x*=-1.0;
    //return abs(vec4(ssShadow.x,0.0,0.0,1.0)/1.0);
    //return (vec4(0.0,ssShadow.y,0.0,1.0)/1.0);
    //return abs(vec4( (ssShadow.x + 0.5) * 0.5,0.0,0.0,1.0)/1.0);

    //vec4 shadowColor=blur3(shadowTex, texCoord.xy );
    vec4 shadowColor=blur3(shadowTex, ssShadow.xy );
    //vec4 shadowColor=blur3(shadowTex, ssShadow.xy );
    shadowColor.x = unpackToFloat(shadowColor.rg) * 512.0;

    //vec4 shadowColor=texture2D(shadowTex, ssShadow.xy );

    //return abs(vec4(shadowColor.r)*10.0);

    if (ssShadow.x<1.0 && ssShadow.x > 0.0 && ssShadow.y<1.0 && ssShadow.y >0.0){
            float falloff = shadowCoord.z - shadowColor.x;
            if (falloff<1.0)
                myLight+= ( min (1.0,max( 0.0,(0.1 *shadowColor.x-falloff)/(0.1*shadowColor.x) ) ) ) * computeLight( );
                myLight *=1.0-(abs (ssShadow.x-0.5) * 2.0);
                myLight *=1.0-(abs (ssShadow.y-0.5) * 2.0);
    }else
            myLight+= vec4(0.0);
  return myLight;
}


void main(){


      for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);

    getPixelLoc();
    //load old lighting data
    gl_FragColor=texture2D(tex, texCoord);

    //gl_FragColor+=computeLight();

    gl_FragColor+=shadowMapping();

    //gl_FragColor.a=1.0;
}
