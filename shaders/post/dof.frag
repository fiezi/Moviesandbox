uniform float time;

uniform float scene_size;
uniform float screenX;
uniform float screenY;

uniform float farClip;

uniform sampler2D tex;
uniform sampler2D depthTex;

uniform bool bSmudge;
uniform float focus;

varying vec2 texCoord;

// Depth of Field variables

    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;

/***********************************************

    Depth of Field

***********************************************/


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

/*
*   5x5 Kernel Gaussian Blur
*/

vec4 blur(sampler2D myTex,vec2 tc, float bias){

      //float spread=1.0/screenX  * (1.0 + (min(6.0,max(2.0,  abs(  unpackToFloat(texture2D(depthTex,texCoord,1.0).xy)  - focus/farClip) )*100.0 )  * unpackToFloat(texture2D(depthTex,texCoord,1.0).xy) )  );
      float spread=2.0/screenX ;// * min(8.0,max(4.0,unpackToFloat(texture2D(depthTex,texCoord).xy*512/50.0)));

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
        sample[i]=texture2D(myTex , tc + tc_offset[i],bias);
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

/*
*   3x3 Kernel Gaussian Blur
*/

vec4 blur3(sampler2D myTex, vec2 tc){

      vec4 sample[9];

      float spread=1.0/screenX  * min(6.0,max(2.0,  abs(focus -  unpackToFloat(texture2D(depthTex,texCoord,1.0).xy))*farClip/20.0)  );
      //float spread=2.0/screenX ;

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
            sample[i]=texture2D(myTex , tc + tc_offset[i],1.5);
      }

      vec4 blurredColor=(
                         sample[0] + (2.0* sample[1]) + sample[2] +
                         (2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
                         sample[6] + (2.0* sample[7]) + sample[8]
                        )/ 13.0;
      //blurredColor.a=1.0;
      return(blurredColor);
}


/*
*   compute Depth of Field
*/

vec4 computeDOF() {

    float depthValue= unpackToFloat(texture2D(depthTex, texCoord).rg)*farClip;

    vec4 blurPart=blur(tex, texCoord,3.0);

    vec4 sharpPart=  texture2D(tex,texCoord,0.0);
    sharpPart.a=1.0;

    //focus = 15.0+ 15.0 * sin(time * 0.0004);
    //focus=32.0;

  //non-linear focus
/*
    float combineBack =min(1.0,depthValue.a/focus);
    float combineFront = min(1.0,focus/depthValue.a);

    float combine = min(combineFront * combineFront , combineBack * combineBack);
    //combine = min(combineFront , combineBack);
    combine =clamp (combine, 0.0, 1.0);
*/

  //linear focus
    float focusDepth=focus/5.0 +1.0;
    //get depth distance to focus:
    float combine=abs(depthValue-focus);
    //clamp focus:
    combine=min(focusDepth,combine);


    vec4 dofColor = combine/focusDepth* blurPart + (1.0-combine/focusDepth) * sharpPart;
     //+ combine * sharpPart ;
    //dofColor=vec4(depthValue.a/100.0);
    dofColor.a=1.0;

    return dofColor;
}



/*
*   Main
*/

void main(void){

    gl_FragColor=computeDOF();

    //gl_FragColor=vec4(   abs(unpackToFloat(texture2D(depthTex,texCoord,1.0).xy)  - focus/farClip ) );
       //gl_FragDepth=texture2D(depthTex,texCoord).r;
    gl_FragColor.a=1.0;

}
