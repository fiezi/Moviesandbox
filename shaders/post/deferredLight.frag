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

uniform vec3 camLoc;
uniform vec3 camX;
uniform vec3 camY;
uniform vec3 camZ;


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

vec4 fragWorldLoc;
float zPos;
float zPosScreen;

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


vec4 calcFragmentWorldSpace(){

    //zPos in Eye Space
    //zPos = texture2D(depthTex,texCoord ).r;
    zPos = texture2D(depthTex,texCoord ).r;

      //  ( (mousePos[1]* 256.0)+ 256.0 * (mousePos[2]* 256.0) )* 1000.0/65536.0;
    //zPos in Screen Space - for reference
    zPosScreen = farClip / (farClip - zPos * (farClip - nearClip));


    //world space
    vec4 fW=vec4(1.0);
    fW.xyz= camLoc;
    fW.xyz+= camZ * (zPos);
    fW.xyz-= camX * ((gl_FragCoord.x/scene_size - 0.5) * zPos * 1.1);
    fW.xyz+= camY * ((gl_FragCoord.y/scene_size - 0.5) * zPos * 768.0/scene_size * 1.1);

    //fW.y-=camLoc.y;
    return fW;
}


vec4 computeLight(){

    //add all previous lighting calculations (from other lights) here:
    vec4 colorLight=gl_LightSource[0].ambient*texture2D(tex, texCoord * lighting_size/scene_size);

    //light in world space
    vec4 lightPos=gl_LightSource[0].position;
    lightPos.w=1.0;

    //transform both to eye space
    vec4 fragWorld= cameraMatrix * fragWorldLoc;

    //fragWorld.z=abs(fragWorld.z)/10.0;
    //return (vec4(fragWorld.z,fragWorld.z,fragWorld.z,1.0)/10.0 );


    //vec4 fragWorld= fragWorldLoc;
    lightPos = cameraMatrix * lightPos;
    //lightPos = cameraInverse * lightPos;
    //lightPos/=lightPos.w;

    vec3 distVec=(lightPos.xyz - fragWorld.xyz);
    //distVec.z*=2.0;
    float dist=length(distVec);

    //black if out of range

    if (dist>gl_LightSource[0].linearAttenuation)
        return vec4(0.0,0.0,0.0,1.0);

    //exaggerate normals
    //zPos=zPosScreen * lightPos.y * 1.0;
    zPos=zPos * 32.0;
    //zPos=dist * 32.0;

    //normal in Eye Space
    vec3 NN= normalize(vec3(dFdx(zPos), dFdy(zPos),1.0));

    vec4 debug=vec4(0.0,0.0,0.0,1.0);
    debug.xyz=fragWorldLoc.xyz;
    debug.xyz=normalize(camZ);
    //return debug;


	vec3 lightCol = gl_LightSource[0].diffuse.rgb;

    //diffuse
    vec3 NL = normalize( distVec.xyz );
	float NdotL = max(0.0,dot(NN,NL));
	colorLight.rgb += 1.0 * lightCol * NdotL;

    //specular
	if( NdotL > 0.0 ){
		vec3 NH = normalize( NL +vec3(0.0,0.0,1.0) );
        colorLight.rgb += 1.0 * lightCol * pow(max(0.0,dot(NN,NH)),specularExp);
	}

    //falloff
    float falloff = (gl_LightSource[0].linearAttenuation-dist)/gl_LightSource[0].linearAttenuation;
    colorLight= colorLight * falloff;

    return colorLight;
}

vec4 shadowMapping(){

    vec4 myLight=vec4(0.0,0.0,0.0,0.0);

   if (gl_LightSource[0].spotCutoff==0.0){
        myLight+=computeLight( );
        return myLight;
    }




	//vec4 pixelPosition=texture2D(pickTex,texCoord );
	vec4 pixelPosition=fragWorldLoc;
    pixelPosition.w=1.0;



    //Matrix transform to light space - our pixel
    vec4 shadowCoord =   lightProjectionMatrix * lightViewMatrix * pixelPosition ;

    vec4 ssShadow=shadowCoord/shadowCoord.w;
    ssShadow=ssShadow * 0.5 + 0.5;

    if (ssShadow.x<1.0 && ssShadow.x > 0.0 && ssShadow.y<1.0 && ssShadow.y >0.0){

        //this leads to hard edges. Maybe we can soften them up a bit?

            //vec4 shadowColor=blur3(shadowTex, ssShadow.xy);
            vec4 shadowColor=texture2D(shadowTex, ssShadow.xy);
            float falloff = (shadowCoord.z) - shadowColor.x;
            //myLight +=max(0.0,(1.0 - falloff))	* computeLight();
            //myLight += computeLight();
            //if (falloff>0.0)
                //myLight+=vec4(1.0);
                //myLight.x=shadowCoord.z/20.0;
			myLight+= ( min (1.0,max( 0.0,(0.1 *shadowColor.a-falloff)/(0.1*shadowColor.a) ) ) ) * computeLight( );
    }

  return myLight;
}


void main(){


      for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);


    fragWorldLoc=calcFragmentWorldSpace();
    //load old lighting data
    gl_FragColor=texture2D(tex, texCoord * lighting_size/scene_size);
    //gl_FragColor=vec4(0.0,0.0,0.0,1.0);
    //gl_FragColor+=computeLight();
    gl_FragColor+=shadowMapping();
    //gl_FragColor.r+=texture2D(depthTex, texCoord).a/100.0;
    //gl_FragColor.r=1.0;
    //gl_FragColor.a=1.0;
}
