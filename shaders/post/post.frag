uniform float time;
uniform float lighting_size;
uniform float scene_size;
uniform float screenX;
uniform float screenY;

uniform bool bSSAO;
uniform bool bLighting;
uniform bool bSmudge;
uniform bool bDrawColor;

uniform sampler2D tex;
uniform sampler2D depthTex;
uniform sampler2D pickTex;
uniform sampler2D shadowTex; // rendered shadow texture
uniform sampler2D fxTex; //rendered picking texture

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraInverse;
uniform mat4 projectionInverse;

varying vec2 texCoord;

// Depth of Field variables

    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;

//Ambient Occlusion variables:



	float d;


	float pw =  1.0/screenX ;  //stepping horizontal
	float ph =  1.0/screenY ;  //stepping vertical


	float ao = 0.0;                 //ambient AO

    float samples = 6.0;            //number of sample circles
	float circleRes= 8.0;           //samples per circle
    float stepsize = 2.0;          //distance of next point in pixels

	float aoMultiplier=10.0;        //progressive darkening
	float falloff =1.015;

    float minDepth=0.05;           //minimum distance to take into account
    float maxDepth=10.0;            //maximum distance to take into account


    float aspect =45.0;             //field of view ratio

    float objectID=0.0;




vec4 blur3(sampler2D myTex, vec2 tc){

      vec4 sample[9];

      //float spread=1.0/600.0  * 8.0/texture2D(myTex , tc).a;
      float spread=0.5/screenX;

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
        if (ceil(texture2D(pickTex , tc + tc_offset[i]).a) == objectID)
            sample[i]=texture2D(myTex , tc + tc_offset[i]);
        else
            sample[i]=texture2D(myTex , tc);
      }

      vec4 blurredColor=(
                         sample[0] + (2.0* sample[1]) + sample[2] +
                         (2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
                         sample[6] + (2.0* sample[7]) + sample[8]
                        )/ 13.0;
      //blurredColor.a=1.0;
      return(blurredColor);
}



/***********************************************

    TODO: move SSAO into deferredLight shader!
    Ambient Occlusion

***********************************************/

/*
*   read Pixel Info
*/

vec4 readPixelInfo( in vec2 coord ) {

    return  texture2D( depthTex, coord ) ;

}

vec4 readObjectInfo( in vec2 coord ) {

    return  texture2D( pickTex, coord ) ;

}
/*
*   compare Ambient Occlusion Samples
*/

void compareAOSamples(float depth, vec3 n1, int j){

    float dist,  l, l2;
    vec3 nDist, vDist, v1, v2, n2;

    //vec4 pixelInfo=readPixelInfo( vec2(texCoord.x+ sin(float(j) * PI/circleRes ) * pw,texCoord.y+ cos(float(j) * PI/circleRes ) * ph));
    vec4 pixelInfo=readPixelInfo( vec2(texCoord.x + pw,texCoord.y + ph));

    n2=pixelInfo.rgb;
    d=pixelInfo.a;

    n1=normalize(n1);
    n2=normalize(n2);
    float cutOff=0.99;

    //this is done because our normals are not 100% perfect.
    //so we need to be more lenient when it comes to self-occlusion!
/*
    int compareObjectID=ceil(texture2D(pickTex,vec2(texCoord.x + pw,texCoord.y + ph)).a);
    if (objectID==compareObjectID){
        cutOff=0.9;
    }
*/

    if ( abs(dot(n2,n1))< cutOff && abs(depth-d)<abs(d/depth))
        ao+=max(1.0 * aoMultiplier * (depth-d)/depth,0.0);
        //ao+=aoMultiplier;
    return;
}

/*
*   compute AO
*/

vec4 computeAO(){

    vec4 pixelInfo=readPixelInfo( texCoord );

    float depth = pixelInfo.a;
    vec3 n1=pixelInfo.xyz;

    if (floor(readObjectInfo(texCoord).a)<0.0)
        return vec4(1.0,1.0,1.0,1.0);


    for (int i=1; i<int(samples+1.0); i++){

        for (int j=0; j<int(circleRes);j++){

            pw= cos(float(j)/(circleRes-1.0) * (2.0 *PI)) * float(i) * stepsize/screenX;
            ph= sin(float(j)/(circleRes-1.0) * (2.0 *PI)) * float(i) * stepsize/screenY;

            compareAOSamples(depth, n1, j);
        }

        aoMultiplier/=falloff;
    }

	ao/= (samples* stepsize);

	vec4 aoColor = vec4(1.0-ao);
	//aoColor =  vec4(depth/100.0);
	//aoColor =  vec4(ao);
	//aoColor.a=1.0;

	return aoColor;
}


/*
*	Smudge FX
*/

vec4 smudge(vec2 coord){


		float smudgeSamples=16.0;
		float myStep=1.0/scene_size;
        myStep=myStep* texture2D(depthTex, texCoord).a;

		vec2 smudge=texture2D(fxTex,coord).xy;

        //if (length(smudge)<0.1)
        //    smudge=vec2(0.2,0.1);


		vec4 smudgeColor=gl_FragData[0];// * texture2D(shadowTex,texCoord);


		if (objectID<0.0)
			return gl_FragData[0];

		for (int i=0;i<int(smudgeSamples);i++){
			vec2 myCoord = vec2(texCoord.x + float(i) * smudge.x * myStep,texCoord.y + float(i) * smudge.y * myStep);
			vec2 myNegCoord = vec2(texCoord.x - float(i) * smudge.x * myStep,texCoord.y - float(i) * smudge.y * myStep);
            smudgeColor+=texture2D(tex, myCoord) * texture2D(shadowTex,myCoord* lighting_size/scene_size);
            smudgeColor+=texture2D(tex, myNegCoord) * texture2D(shadowTex,myNegCoord* lighting_size/scene_size);
		}
		smudgeColor*=1.0/(smudgeSamples * 2.0);
		smudgeColor.a=1.0;

		//debug
		//smudgeColor.xy=smudge;
		//smudgeColor.b=0.0;

		return smudgeColor;
}

/*
*   Main
*/

void main(void){


    ///color map
    if (bDrawColor)
        gl_FragData[0]=texture2D(tex, texCoord);
    else
    ///lighting only
        gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

    objectID=ceil(texture2D(pickTex,texCoord).a);

    ///regular shadows
    //if we have negative values in our first channel, we are unlit!
    if (bLighting){// && !bSmudge){
        //vec4 lightData=texture2D(shadowTex,texCoord);
        vec4 lightData=texture2D(shadowTex,texCoord);
        gl_FragData[0]*=lightData;
    }

    ///Ambient Occlusion
    //if (bSSAO)
     //   gl_FragData[0].rgb*=computeAO().rgb ;

   // gl_FragDepth=texture2D(pickTex,texCoord).r;

    ///debug stuff
/*
    if (gl_FragCoord.x>screenX/2.0-1.0 && gl_FragCoord.x<screenX/2.0+1.0 )
        gl_FragData[0].r=1.0;

    if (gl_FragCoord.y>screenY/2.0-1.0 && gl_FragCoord.y<screenY/2.0+1.0 )
        gl_FragData[0].r=1.0;
*/
    //gl_FragData[0]/=3.0;
    //gl_FragData[0].rgb=texture2D(shadowTex, texCoord).rgb;
    //gl_FragData[0].g+=0.0001 * texture2D(pickTex, texCoord).g;
    //gl_FragData[0].b+=0.0001 * texture2D(depthTex, texCoord).b;
    //gl_FragData[0].a=1.0;
    //gl_FragData[0].rgb=texture2D(depthTex, texCoord).rgb;
    //gl_FragData[0].rgb=texture2D(depthTex, texCoord).a/100.0;
    //gl_FragData[0].b=texture2D(depthTex, texCoord).a/100.0;
    //vec3 norm=readNormal(texCoord);
    //gl_FragData[0].xyz+=0.10 * norm;

    //gl_FragData[0].r=1.0;
    //gl_FragData[0].a=1.0;
}
