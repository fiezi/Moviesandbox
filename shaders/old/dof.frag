uniform sampler2D tex; // rendered scene
uniform sampler2DShadow shadowTex; // rendered shadow texture
uniform sampler2D depthTex; // rendered depth texture
varying vec2 texCoord;
uniform float time;
vec2 camerarange;
vec2 screensize;

vec2 tc_offset[25];

vec4 blur(){

  float spread=1.0/220.0;
  
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
    sample[i]=texture2D(tex , texCoord + tc_offset[i]);
  }
  
  vec4 blurredColor=(
                    (1.0 * (sample[0] + sample[4] + sample[20] + sample[24])) + 
                    
                    (4.0 * (sample[1] + sample[3] + sample[5] + sample[9] +
                           sample[15] + sample[19] + sample[21] + sample[23])) + 
                    
                    (7.0 * (sample[2] + sample[10] + sample[14] + sample[22])) +
                    
                    (16.0 * (sample[6] + sample[8] + sample[16] + sample[18])) +
                    
                    (26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
                    (41 * sample[12])
                    )/ 273.0;
  blurredColor.a=1.0;                  
  return(blurredColor);
}

void main(void){

  vec4 depthValue= texture2D(depthTex, texCoord);
  
  vec4 blurPart=blur();
  
  vec4 sharpPart=  texture2D(tex,texCoord);
  sharpPart.a=1.0;
  
  float focus = 15.0 * sin(time * 0.0001);
  focus = 120.5;
  float combineBack =min(1.0,depthValue.r/focus);
  float combineFront = min(1.0,focus/depthValue.r);
  
  float combine = min(combineFront , combineBack);
  combine = min(combineFront *  combineFront, combineBack * combineBack);
  //combine =clamp (combine, 0.0, 1.0);
  gl_FragColor = (1.0-combine)* blurPart + combine * sharpPart ; 
     
  
	gl_FragColor.a=1.0;
}
