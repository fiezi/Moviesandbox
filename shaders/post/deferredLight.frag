uniform float time;
uniform float screensize;
uniform float lighting_size;
uniform float shadow_size;
uniform float scene_size;

uniform float nearClip;
uniform float farClip;

uniform sampler2D tex; // rendered scene 512*512 (lighting resolution)
uniform sampler2D depthTex; // rendered normals and depth texture
uniform sampler2D pickTex; //rendered picking texture -1024*1024 (scene resolution)
uniform sampler2D shadowTex; // rendered shadow textures - 256x256 (shadow resolution)

uniform mat4 lightProperties;

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraInverse;
uniform mat4 projectionInverse;

varying vec2 texCoord;

float lightDistance = 100.0;

const float specularExp = 32.0;



// Depth of Field variables

    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;


vec4 blur3(sampler2D myTex, vec2 tc){

      vec4 sample[9];

      float spread=1.0/screensize;//   * texture2D(myTex , tc).a/32.0;

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


vec4 computeLight(){

    //add all previous lighting calculations (from other lights) here:
    vec4 colorLight=gl_LightSource[0].ambient*texture2D(tex, texCoord * lighting_size/scene_size);

    //calculate distance
    //light in world space
    vec4 lightPos=gl_LightSource[0].position;
    lightPos.w=1.0;
    //convert to eye space
    lightPos=projectionMatrix * cameraMatrix * lightPos;
    //lightPos=projectionInverse * cameraInverse * lightPos;
    //lightPos=lightPos/scene_size;
    //convert to screen space
    lightPos=lightPos/lightPos.w;
    lightPos=lightPos* 0.5 +0.5;
//    lightPos.a=1.0;


    float zPos = blur3(depthTex,texCoord ).r ;

    //bring z from view space to screen space
    //depthEye = (zNear * zFar) / (zFar - depthScreen * (zFar - zNear));
    //zFar - depthScreen * (zFar - zNear) = zNear * zFar/depthEye;
    //-depthScreen* (zFar -zNear)= (zNear * zFar/depthEye) - zFar;
    //float zPosScreen=-((nearClip * farClip/zPos ) - farClip) /(farClip -nearClip);
    //zPosScreen=zPos/1000.0;
    float zPosScreen=(nearClip * farClip) / (farClip - zPos * (farClip - nearClip));
    //zPosScreen=zPos/100.0;
    //zPos=zPosScreen;
    //zPos= zPos * 0.5 +0.5;
    //frag Position in screen space
    vec4 fragmentPos=vec4(gl_FragCoord.x,gl_FragCoord.y,zPosScreen,1.0);
    fragmentPos.xy=fragmentPos.xy/scene_size;

    //return (vec4(zPosScreen,0,0,1)/2.0);

    //distance calculations

    vec3 distVec = lightPos.xyz - fragmentPos.xyz;
    //vec3 distVec = fragmentPos.xyz - lightPos.xyz;

    //distVec= distVec* cameraInverse;

    float dist=distance(lightPos.xyz,fragmentPos.xyz);
    //float dist=distance(fragmentPos,lightPos);

    //return vec4(1.0,1.0,1.0,1.0)*abs(dist)/30.0;

    if (abs(dist)>gl_LightSource[0].linearAttenuation)
        return vec4(0.0,0.0,0.0,0.0);

    float linAtt = (gl_LightSource[0].linearAttenuation-abs(dist))/gl_LightSource[0].linearAttenuation;


    zPos*=100.0;
	//calculate Normal from screen space difference to adjacent fragment
    vec3 NN=normalize(vec3(dFdx(zPos),dFdy(zPos),1.0));


	vec3 lightCol = gl_LightSource[0].diffuse.rgb;

    //diffuse
	vec3 NL = normalize( distVec.xyz );
	float NdotL = max(0.0,dot(NL,NN));
	colorLight.rgb += 1.0 * lightCol * NdotL;

    //specular
	if( NdotL > 0.0 ){
		vec3 NH = normalize( NL +vec3(0.0,0.0,1.0) );
        colorLight.rgb += 1.0 * lightCol * pow(max(0.0,dot(NN,NH)),specularExp);
	}

    colorLight= colorLight * linAtt;

    return colorLight;
}

vec4 shadowMapping(){

    vec4 myLight=vec4(0.0,0.0,0.0,0.0);

   if (gl_LightSource[0].spotCutoff==0.0){
        myLight+=computeLight( );
        return myLight;
    }




	vec4 pixelPosition=texture2D(pickTex,texCoord );
    pixelPosition.w=1.0;



    //Matrix transform to light space - our pixel
    vec4 shadowCoord =   lightProjectionMatrix * lightViewMatrix * pixelPosition ;

    vec4 ssShadow=shadowCoord/shadowCoord.w;
    ssShadow=ssShadow * 0.5 + 0.5;

    if (ssShadow.x<1.0 && ssShadow.x > 0.0 && ssShadow.y<1.0 && ssShadow.y >0.0){

        //this leads to hard edges. Maybe we can soften them up a bit?

            //vec4 shadowColor=blur3(shadowTex, ssShadow.xy);
            vec4 shadowColor=texture2D(shadowTex, ssShadow.xy);
            float falloff = (shadowCoord.z) - shadowColor.a;
			//myLight +=max(0.0,(1.0 - falloff))	* computeLight();
			myLight+= ( min (1.0,max( 0.0,(0.1 *shadowColor.a-falloff)/(0.1*shadowColor.a) ) ) ) * computeLight( );
    }

  return myLight;
}


void main(){


      for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);


    //load old lighting data
    gl_FragColor=texture2D(tex, texCoord * lighting_size/scene_size);
    //gl_FragColor.g+=0.0001 * texture2D(pickTex, texCoord).r;
    //gl_FragColor.b+=0.0001 * texture2D(depthTex, texCoord).g;
    //gl_FragColor.b+=0.0001 *texture2D(shadowTex,texCoord).b;
    //gl_FragColor+=computeLight();
    gl_FragColor+=shadowMapping();
    //gl_FragColor.r+=texture2D(depthTex, texCoord).a/100.0;
    //gl_FragColor.r=1.0;
    //gl_FragColor.a=1.0;
}
