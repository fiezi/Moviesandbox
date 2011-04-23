uniform float time;

uniform float scene_size;
uniform sampler2D tex;
uniform sampler2D depthTex;
uniform sampler2D pickTex;

uniform float focus;

varying vec2 texCoord;

// Depth of Field variables

    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;

/***********************************************

    Depth of Field

***********************************************/

/*
*   5x5 Kernel Gaussian Blur
*/

vec4 blur(sampler2D myTex,vec2 tc){

      float spread=1.0/scene_size  * min(8.0,max(4.0,texture2D(depthTex,texCoord).a/50.0));

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

/*
*   3x3 Kernel Gaussian Blur
*/

vec4 blur3(sampler2D myTex, vec2 tc){

      vec4 sample[9];

      float spread=1.0/1024.0;

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


/*
*   compute Depth of Field
*/

vec4 computeDOF() {

    vec4 depthValue= texture2D(depthTex, texCoord);

    if (depthValue.a<=0.0)
        depthValue.a=65536.0;

    vec4 blurPart=blur(tex, texCoord);

    vec4 sharpPart=  texture2D(tex,texCoord);
    sharpPart.a=1.0;

    //float focus = 35.0+ 35.0 * sin(time * 0.0001);
    //focus=10.0;

  //non-linear focus
/*
    float combineBack =min(1.0,depthValue.a/focus);
    float combineFront = min(1.0,focus/depthValue.a);

    float combine = min(combineFront * combineFront , combineBack * combineBack);
    //combine = min(combineFront , combineBack);
    combine =clamp (combine, 0.0, 1.0);
*/

  //linear focus
    float focusDepth=10.0;
    //get depth distance to focus:
    float combine=abs(depthValue.a-focus);
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

    //gl_FragColor=texture2D(tex, texCoord);
    gl_FragColor=computeDOF();

}
