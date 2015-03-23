uniform float time;
uniform float screensize;
uniform float lighting_size;
uniform float shadow_size;
uniform float screenX;
uniform float screenY;

uniform float nearClip;
uniform float farClip;

uniform sampler2D tex; // rendered scene
uniform sampler2D depthTex; // rendered depth texture
uniform sampler2D shadowTex; // rendered shadow textures
uniform sampler2D normalTex; // rendered normal texture

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 cameraMatrix;
uniform mat4 cameraInverse;
uniform mat4 projectionMatrix;
uniform mat4 projectionInverse;

uniform mat4 inverseCameraRotationMatrix;
uniform mat4 cameraRotationMatrix;


uniform vec3 camLoc;
uniform vec3 camX;
uniform vec3 camY;
uniform vec3 camZ;


varying vec2 texCoord;

//light position stuff

varying vec3 lightColor;
varying vec4 lightPos;
varying mat4 lightSpaceMat;

const float specularExp = 00.00;

//pixel position stuff
vec4 pixelPos;
float zPos;
float zPosScreen;


// blur variables




    float PI = 3.14159265358979323846264;

float unpackToFloat(vec4 value){

const vec4 bitSh = vec4(1.0 / (255.0 * 255.0 * 255.0), 1.0 / (255.0 * 255.0), 1.0 / 255.0, 1.0);

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


vec4 blur5(sampler2D myTex,vec2 tc){


vec2 tc_offset[25];

for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);

float spread=1.0/screenX;// * min(0.0,max(4.0,unpackToFloat(texture2D(depthTex,texCoord).xy*farClip/100.0)));
      //float spread=2.0/screenX ;// * min(8.0,max(4.0,unpackToFloat(texture2D(depthTex,texCoord).xy*512/50.0)));

      tc_offset[0]=spread * vec2(-2.0,-2.0);
      tc_offset[1]=spread * vec2(-1.0,-2.0);
      tc_offset[2]=spread * vec2(0.0,-2.0);
      tc_offset[3]=spread * vec2(1.0,-2.0);
      tc_offset[4]=spread * vec2(2.0,-2.0);

      tc_offset[5]=spread * vec2(-2.0,-1.0);
      tc_offset[6]=spread * vec2(-1.0,-1.0);
      tc_offset[7]=spread * vec2(0.0,-1.0);
      tc_offset[8]=spread * vec2(1.0,-1.0);
      tc_offset[9]=spread * vec2(2.0,-1.0);

      tc_offset[10]=spread * vec2(-2.0,0.0);
      tc_offset[11]=spread * vec2(-1.0,0.0);
      tc_offset[12]=spread * vec2(0.0,0.0);
      tc_offset[13]=spread * vec2(1.0,0.0);
      tc_offset[14]=spread * vec2(2.0,0.0);

      tc_offset[15]=spread * vec2(-2.0,1.0);
      tc_offset[16]=spread * vec2(-1.0,1.0);
      tc_offset[17]=spread * vec2(0.0,1.0);
      tc_offset[18]=spread * vec2(1.0,1.0);
      tc_offset[19]=spread * vec2(2.0,1.0);

      tc_offset[20]=spread * vec2(-2.0,2.0);
      tc_offset[21]=spread * vec2(-1.0,2.0);
      tc_offset[22]=spread * vec2(0.0,2.0);
      tc_offset[23]=spread * vec2(1.0,2.0);
      tc_offset[24]=spread * vec2(2.0,2.0);


      vec4 sample[25];

      for (int i=0 ; i<25 ; i++)
      {
        sample[i]=texture2D(myTex , tc + tc_offset[i]);
      }

      vec4 blurredColor=(
                        (1.0 * (sample[0] + sample[4] + sample[20] + sample[24])) +

                        (4.0 * (sample[1] + sample[3] + sample[5] + sample[9] +
                               sample[15] + sample[19] + sample[21] + sample[23])) +

                        (7.0 * (sample[2] + sample[10] + sample[14] + sample[22])) +

                        (16.0 * (sample[6] + sample[8] + sample[16] + sample[18])) +

                        (26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
                        (41.0 * sample[12])
                        )/ 273.0;
      blurredColor.a=1.0;
      return(blurredColor);
}


vec4 blur3(sampler2D myTex, vec2 tc, float bias){

vec2 tc_offset[25];

for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);

vec4 sample[9];

      float spread=1.0/screenX;// * unpackToFloat(texture2D(myTex , tc).rg)*farClip/16.0;
      //float spread=0.250/shadow_size;// * texture2D(myTex , tc).a/32.0;

      tc_offset[0]=spread * vec2(-1.0,-1.0);
      tc_offset[1]=spread * vec2(0.0,-1.0);
      tc_offset[2]=spread * vec2(1.0,-1.0);

      tc_offset[3]=spread * vec2(-1.0,0.0);
      tc_offset[4]=spread * vec2(0.0,0.0);
      tc_offset[5]=spread * vec2(1.0,0.0);

      tc_offset[6]=spread * vec2(-1.0,-1.0);
      tc_offset[7]=spread * vec2(0.0,-1.0);
      tc_offset[8]=spread * vec2(1.0,-1.0);


sample[0]=texture2D(myTex ,tc + tc_offset[0], bias );
sample[1]=texture2D(myTex ,tc + tc_offset[1], bias);
sample[2]=texture2D(myTex ,tc + tc_offset[2], bias );
sample[3]=texture2D(myTex ,tc + tc_offset[3], bias);
sample[4]=texture2D(myTex ,tc + tc_offset[4], bias );
sample[5]=texture2D(myTex ,tc + tc_offset[5], bias );
sample[6]=texture2D(myTex ,tc + tc_offset[6], bias );
sample[7]=texture2D(myTex ,tc + tc_offset[7], bias );
sample[8]=texture2D(myTex ,tc + tc_offset[8], bias );




      vec4 blurredColor=(
                         sample[0] + (2.0* sample[1]) + sample[2] +
                         (2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
                         sample[6] + (2.0* sample[7]) + sample[8]
                        )/ 13.0;


      return blurredColor;
}



///pixelPosition in eyeSpace ( pixel on ground plane stays the same independent of camera's distance to ground plane )
void getPixelLoc(){

    //zPos = texture2D(depthTex,texCoord).r;

    vec2 tc=texCoord;

    zPos= unpackToFloat(blur3(depthTex,tc,1.0).rg) * (farClip);

    //zPos= unpackToFloat(texture2D(depthTex,tc).rg) * (farClip);

    pixelPos.z=(1.0-zPos);
    //pixelPos.z=zPos;
    pixelPos.y=(gl_FragCoord.y/screenY - 0.5) * 720.0/768.0;
    pixelPos.x=(gl_FragCoord.x/screenX - 0.5) * 1280.0/1024.0;

    pixelPos.xy*=pixelPos.z;



    pixelPos.z-=1.0;

}



vec4 computeLight(){



    //add all previous lighting calculations (from other lights) here:
    //vec4 colorLight=0.5*texture2D(tex, texCoord);
    vec4 colorLight=gl_LightSource[0].ambient*texture2D(tex, texCoord);

    vec4 pixelNormal=vec4(0,0,0,0);
    pixelNormal.xyz=texture2D(normalTex,texCoord,0.0).xyz;

    pixelNormal-=0.5;
    pixelNormal*=2.0;
    //pixelNormal.xy*=zPos;
    pixelNormal.w=0.0;
    pixelNormal.xyz=normalize(pixelNormal.xyz);


    vec3 pp=-pixelPos.x *camX + pixelPos.y*camY + pixelPos.z *camZ - camLoc;

    vec4 lightDirection=vec4(1.0,0.0,0.0,0.0);
    lightDirection.xyz=pp.xyz+lightPos.xyz;
    vec3 lightDirectionNormalized=normalize(lightDirection.xyz);


    float NdotL=max(0.0,  dot(pixelNormal.xyz, lightDirectionNormalized )  );

    //lightDirection.z/=10.0;
    float dist= length(lightDirection);
    float att=max( 0.0 , (gl_LightSource[0].linearAttenuation - dist)/gl_LightSource[0].linearAttenuation );

    colorLight.xyz=att * NdotL  *1.0 * lightColor;
    //colorLight.xyz= NdotL  *1.0 * lightColor;


    if (NdotL>0.0 && specularExp >0.0){
    vec3 NH = normalize(lightDirectionNormalized - camZ  );
    colorLight.xyz*=1.0 * lightColor + pow(max(0.0, dot(pixelNormal,NH)),specularExp   );
    }



    //return vec4(1.0);
    return colorLight;


}



vec4 shadowMapping(){


    // return(vec4(zPos));
    //vec4 myLight=gl_LightSource[0].ambient*texture2D(tex, texCoord);
    vec4 myLight=vec4(0.0);

   if (gl_LightSource[0].spotCutoff==0.0){
        myLight+=computeLight( );
        return myLight;
    }


    //where do these numbers come from? and what do they want from us?
    vec4 pixelPosition=vec4((texCoord.x-0.5)* 0.835 * screenX/screenY, (texCoord.y-0.5)* 0.835, (-zPos) * 1.0, 1.0 ) ;
    pixelPosition.xy*=zPos;


    //Matrix transform to light space - our pixel. Matrices are computed once and can be done in CPU or vertex shader
    vec4 shadowCoord = lightSpaceMat*pixelPosition ;
   vec2 ssShadow=shadowCoord.xy/shadowCoord.w;


        ssShadow=(ssShadow* 0.5) + 0.5;

    //vec4 shadowColor=texture2D(shadowTex, ssShadow.xy,0.0 );
    vec4 shadowColor=blur3(shadowTex, ssShadow.xy,3.0 );
    shadowColor.x = unpackToFloat(shadowColor.rg) * farClip;

    if (ssShadow.x<1.0 && ssShadow.x > 0.0 && ssShadow.y<1.0 && ssShadow.y >0.0){
            float falloff = shadowCoord.z - shadowColor.x;
            if (falloff<1.0 ){
                myLight+= abs( min (1.0,max( 0.0,(1.0 *shadowColor.x-falloff)/(1.0*shadowColor.x) ) ) ) * computeLight( );
                myLight *=1.0-abs( (ssShadow.x-0.5) * 2.0);
                //myLight *=1.0-abs( (ssShadow.y-0.5) * 2.0);
            }
    }else
            myLight+= 0.0;
  return myLight;
}


void main(){




    getPixelLoc();
    //add old lighting data
    gl_FragColor= texture2D(tex, texCoord) + shadowMapping();
    //gl_FragColor.r=min(1.15,gl_FragColor.r);
    //gl_FragColor.g=min(1.15,gl_FragColor.g);
    //gl_FragColor.b=min(1.15,gl_FragColor.b);
    //gl_FragColor= shadowMapping();
    //gl_FragColor= texture2D(tex, texCoord) + computeLight();
    //gl_FragColor= computeLight() * shadowMapping();
    //gl_FragColor= vec4(1,0,0,1);


}
