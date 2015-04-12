
#extension GL_EXT_gpu_shader4 : enable

uniform float time;
uniform float lighting_size;
uniform float farClip;
uniform float screenX;
uniform float screenY;
uniform float fov;

uniform bool bSSAO;
uniform bool bLighting;
uniform bool bGlitch;
uniform bool bSmudge;
uniform bool bDrawNormals;
uniform bool bDrawColor;
uniform bool bInvert;

uniform sampler2D tex;
uniform sampler2D depthTex;
uniform sampler2D shadowTex; // rendered shadow texture
uniform sampler2D normalTex; //screen space normals

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
	float falloff =1.00015;

    float minDepth=0.005;           //minimum distance to take into account
    float maxDepth=10000.0;            //maximum distance to take into account


    float aspect =fov;             //field of view ratio

    float objectID=0.0;


vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}


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



/***********************************************

    TODO: move SSAO into deferredLight shader!
    Ambient Occlusion

***********************************************/



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



/*
*   read Pixel Info
*/

vec4 readPixelInfo( in vec2 coord ) {

   return  vec4(texture2D( normalTex, coord ).xyz,  unpackToFloat(texture2D( depthTex, coord ).rg)* farClip );

}

float readObjectInfo( in vec2 coord ) {

    return  unpackToFloat(texture2D( depthTex, coord ).ba)*1024.0 - 100.0 ;

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

    if (floor(readObjectInfo(texCoord))<0.0)
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

/*
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
		*/
		return vec4(1.0);
}

vec3 glitch(sampler2D glitchTex, vec2 tc, float bias, float amount){

vec3 returnColor=vec3(0.0);

returnColor.r=texture2D(glitchTex, tc   +amount* vec2(0.01 * snoise(vec2(time*0.00001,gl_FragCoord.y*0.00005 + time*0.0001)),
                                        +0.001 * max(snoise(vec2(time*0.001,gl_FragCoord.y*0.005 + time*0.0001)),0.2)
                                                * 40.0 * snoise(vec2(time*0.1,time*0.001+ gl_FragCoord.y))
                                              ),
                        0.0* amount * max(snoise(vec2(time*0.01,gl_FragCoord.y*0.1+ time*0.001)),0.0)
                             ).r;

returnColor.g=texture2D(glitchTex, tc   + amount * vec2(0.01*snoise(vec2(time*0.00001,gl_FragCoord.y*0.0005 + time*0.00001)),
                                        +0.02 * max(snoise(vec2(time*0.000115,gl_FragCoord.y*0.00005 + time*0.001)),-0.2)
                                                * 1.0 * snoise(vec2(time*0.001,time*0.1+ gl_FragCoord.y))
                                              ),
                        0.0* amount * max(snoise(vec2(time*0.0001,gl_FragCoord.y*0.0005 + time*0.00001)),-0.2)
                             ).g;

returnColor.b=texture2D(glitchTex, tc   + amount * vec2(0.001* snoise(vec2(time*0.0001,gl_FragCoord.y*0.0005 + time*0.001)),
                                        +0.01 * max(snoise(vec2(time*0.00012,gl_FragCoord.y*0.0005 + time*0.00001)),-0.2)
                                                * 60.0 * snoise(vec2(0.0,time*0.001+ gl_FragCoord.y))
                                              ),
                        0.0* amount* max(snoise(vec2(time*0.01,gl_FragCoord.y*0.1+ time*0.001)),0.0)

                             ).b;

return returnColor;
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



    ///regular shadows
    //if we have negative values in our first channel, we are unlit!

    if (bLighting){// && !bSmudge){

        vec4 lightData=texture2D(shadowTex,texCoord,0.0) ;
        //vec3 lightData=glitch(shadowTex,texCoord,0.0, 5.0 * snoise(vec2(time * 0.002, time * 0.001))) ;
        //vec4 lightData=blur3(shadowTex,texCoord) ;

        //this gives us the opportunity to "hide" data in the rgb channels
        //here, we check if we are lit or not

        //if  (  (fract(gl_FragData[0].r*100.0)<0.2 || fract(gl_FragData[0].r*100.0)>0.8 ) )
                gl_FragData[0].rgb*=1.0*lightData;
    }

            ///GLITCHING FROM HERE
    if (bGlitch){
        gl_FragData[0].rgb=glitch(tex, texCoord, 0.0, 10.0 * snoise(vec2(time * 0.002, time * 0.001))) *
                           glitch(shadowTex,texCoord,0.0, 30.5 * snoise(vec2(time * 0.002, time * 0.001))) ;
    }


    if (bDrawNormals)
        gl_FragData[0]=texture2D(normalTex,texCoord);

    ///Ambient Occlusion
    if (bSSAO)
       gl_FragData[0].rgb*=computeAO().rgb;

    //gl_FragDepth=texture2D(pickTex,texCoord).r;

    ///debug stuff
/*
    if (gl_FragCoord.x>screenX/2.0-1.0 && gl_FragCoord.x<screenX/2.0+1.0 )
        gl_FragData[0].r=1.0;

    if (gl_FragCoord.y>screenY/2.0-1.0 && gl_FragCoord.y<screenY/2.0+1.0 )
        gl_FragData[0].r=1.0;
*/


    ///GreyScale
    ///desaturate
    float greyValue=(gl_FragData[0].r+gl_FragData[0].g+gl_FragData[0].b)/3.0;
    vec3 desaturate=vec3(greyValue);
    gl_FragData[0].rgb=0.7* desaturate+ 0.3*gl_FragData[0].rgb;

    ///Black Level
    float lowCutOff=0.15;
    if (greyValue< lowCutOff){
        gl_FragData[0].b+=0.051;
        gl_FragData[0].rgb*=0.85;
    }

    ///Max Level
    float highCutOff=0.75;
    if (greyValue> highCutOff)
        gl_FragData[0].rgb*=1.3;


    ///Mid Level
    if (greyValue> lowCutOff && greyValue<highCutOff){
        gl_FragData[0].g*=1.15;
        gl_FragData[0].b*=1.15;
        gl_FragData[0].b+=0.051;
    }

    ///fog... is done in ocean shader...



    ///
    gl_FragData[0]=gl_FragData[0] * 0.4 + gl_FragData[0] * greyValue * 0.6;

    //if (bInvert)
    //    gl_FragData[0].rgb=1C:\Moviesandbox\config.xml.0-gl_FragData[0].rgb;


    //gl_FragData[0]/=3.0;
    //gl_FragData[0].rgb=texture2D(shadowTex, texCoord).rgb;
   // gl_FragData[0].g+=0.0001 * texture2D(pickTex, texCoord).r;
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
    //gl_FragData[0].r=texture2D(pickTex,texCoord).r;



}
