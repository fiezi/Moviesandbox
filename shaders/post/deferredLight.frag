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
uniform mat4 lightViewMatrixInverse;
uniform mat4 lightProjectionMatrix;
uniform mat4 lightProjectionMatrixInverse;
uniform mat4 cameraMatrix;
uniform mat4 cameraInverse;
uniform mat4 projectionMatrix;
uniform mat4 projectionInverse;


uniform vec3 camLoc;
uniform vec3 camX;
uniform vec3 camY;
uniform vec3 camZ;


varying vec2 texCoord;
varying vec2 shadowCoord;

//light position stuff

varying vec3 lightColor;
varying vec4 lightPos;
varying float lightZScreen;

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


///pixelPosition in eyeSpace ( pixel on ground plane stays the same independent of camera's distance to ground plane )
void getPixelLoc(){

    //zPos = texture2D(depthTex,texCoord ).r;
    zPos = blur3(depthTex,texCoord ).r;
    zPosScreen=farClip/ (farClip - zPos * (farClip- nearClip));
    pixelPos=(vec4(texCoord.x-0.5, texCoord.y-0.5, zPos,1.0)/zPosScreen)/10.0;
    pixelPos.w=1.0;
}



vec4 computeLight(){

    //add all previous lighting calculations (from other lights) here:
    vec4 colorLight=gl_LightSource[0].ambient*texture2D(tex, texCoord);

    float xDist= (lightPos.x/16.0 - pixelPos.x); //* screenX/screenY;
    float yDist= (lightPos.y/9.0 - pixelPos.y);//* screenY/screenX;
    float zDist= (lightPos.z /10.0- pixelPos.z)/100.0;

    vec3 lightDirection=vec3(-xDist,-yDist,zDist);
    vec3 lightDirectionNormalized =normalize(lightDirection);
    float lightDistance=length(lightDirection)*10.0;

    //return here if we're out of range!
    if (lightDistance>gl_LightSource[0].linearAttenuation)
        return vec4(0.0,0.0,0.0,1.0);

    //normal in Eye Space is the difference in z
    //we exagerrate along z - because differences become more significant with higher z
    float dx= dFdx(zPos)/(zPos*zPos);
    float dy= dFdy(zPos)/(zPos*zPos);
    vec3 pixelNormal=normalize(vec3(dx,dy,0.0001));

    //diffuse is dot Product of lightdirection on pixel normal
	float lightDotPixel = max(0.0,(dot(pixelNormal,lightDirectionNormalized) )  );
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

    vec4 myLight=vec4(0.0,0.0,0.0,0.0);

   if (gl_LightSource[0].spotCutoff==0.0){
        myLight+=computeLight( );
        return myLight;
    }
/*
    vec3 xAxis =vec3(cameraMatrix[0][0],cameraMatrix[1][0], cameraMatrix[2][0]);
    vec3 yAxis =vec3(cameraMatrix[0][1],cameraMatrix[1][1], cameraMatrix [2][1]);
    vec3 zAxis =vec3(cameraMatrix[0][2],cameraMatrix[1][2], cameraMatrix [2][2]);
    vec4 camLoc = cameraMatrix[3];
    camLoc.xyz+=zAxis * zPos;
    camLoc.xyz-=xAxis * (texCoord.x-0.5) * zPos;
    camLoc.xyz+=yAxis * (texCoord.y-0.5) * zPos;
*/

    vec4 fW = vec4(1.0);
    fW.xyz=camLoc;
    fW.xyz+=camZ * zPos;
    fW.xyz-=camX * (texCoord.x-0.5)  * zPos;
    fW.xyz+=camY * (texCoord.y -0.5) * zPos;


    vec4 sCoord = vec4(1.0);

    //convert from screen space to eye space
    sCoord.z=(-texture2D(shadowTex,shadowCoord).r) ;
    float zCoordScreen=farClip/ (farClip - sCoord.z * (farClip- nearClip));

    sCoord.xy=shadowCoord.xy;
    sCoord.xy=(shadowCoord.xy  - 0.5) * 2.0;
    sCoord.xy/=zCoordScreen;
    sCoord.w=1.0;

    //convert to world Space
    //sCoord=lightProjectionMatrixInverse * sCoord;
    sCoord=lightViewMatrixInverse * sCoord;
    //sCoord= lightViewMatrixInverse * sCoord;
    //sCoord=lightProjectionMatrixInverse * sCoord;
    sCoord/=sCoord.w;
    //return abs(vec4(sCoord.x,sCoord.y,sCoord.z,1.0)/10.0);


    //convert to Camera Space
    sCoord=cameraMatrix * sCoord;
    sCoord/=sCoord.w;
    //return abs(vec4(0.0,0.0,sCoord.z-pixelPos.z,1.0)/100.0);

    //sCoord=sCoord * 0.5 + 0.5;

    //return vec4(shadowCoord.x,shadowCoord.y,0,1.0)/1.0;
    //return texture2D(shadowTex,shadowCoord).r/10.0;
    //return vec4(sCoord.x,sCoord.y,sCoord.z,1.0)/1.0;



    //vec4 pixelPosition=vec4((texCoord.x), texCoord.y, zPos, 1.0 );

    //vec4 pixelPosition=vec4((texCoord.x-0.5)*1.0, (texCoord.y-0.5)*1.0, (1.0/zPosScreen)*1.0 , 1.0 )  ;
    //pixelPosition.xy/=(-zPosScreen);

    vec4 pixelPosition=vec4((texCoord.x-0.5)*1.45, (texCoord.y-0.5)* 0.80, (-zPos) * 1.0, 1.0 )  ;
    pixelPosition.xy*=zPos;


    pixelPosition=cameraInverse * pixelPosition;
    pixelPosition/=pixelPosition.w;


    //return abs(vec4(0.0,pixelPosition.y,0.0,1.0)/10.0);
    //return abs(vec4(pixelPosition.x,0.0,0.0/1.0,1.0)/1.0);
    //return abs(vec4(0.0,0.0,pixelPosition.z/1.0,1.0)/1.0);
    //return abs(vec4(pixelPosition.x,pixelPosition.y,pixelPosition.z/1.0,1.0)/10.0);



    //Matrix transform to light space - our pixel
    vec4 shadowCoord =   lightProjectionMatrix * lightViewMatrix *  pixelPosition ;
    //vec4 shadowCoord =    lightViewMatrix * pixelPosition ;
    shadowCoord/=shadowCoord.w;
    //transform light to image space!
    //vec4 ssShadow=lightProjectionMatrix * lightViewMatrix * pixelPosition;

    //return abs(vec4(shadowCoord.x,shadowCoord.y,shadowCoord.z/1.0,1.0)/10.0);

    vec2 ssShadow=shadowCoord.xy;


    ssShadow=ssShadow * 0.5 + 0.5;

    if (ssShadow.x<1.0 && ssShadow.x > 0.0 && ssShadow.y<1.0 && ssShadow.y >0.0){

            //this leads to hard edges. Maybe we can soften them up a bit?

            vec4 shadowColor=blur3(shadowTex, ssShadow.xy );
            //vec4 shadowColor=texture2D(shadowTex, ssShadow.xy);

            //return (vec4(pixelPosition.z - shadowColor.x)*100000000.0);
            float falloff = pixelPosition.z - shadowColor.x;
            //myLight +=max(0.0,(1.0 - falloff))	* computeLight();
            //myLight += computeLight();
            if (falloff<=-12.0)
                //myLight+=vec4(1.0);
                //myLight.x=shadowCoord.z/20.0;
                myLight+= ( min (1.0,max( 0.0,(0.1 *shadowColor.x-falloff)/(0.1*shadowColor.x) ) ) ) * computeLight( );
    }

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
