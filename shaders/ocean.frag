uniform vec4 postColor;
uniform bool bComputeLight;

uniform float objectID;
uniform float screenX;
uniform float screenY;
uniform float farClip;
uniform float nearClip;

varying float zPos;
varying float oID;

/*
*   Main
*/


/*
These are from a discussion on gamedev.net - all credit goes to unknown people at a lost codesampler thread, Ysaneya for explaining and jamesw and gjaegy for reposting
http://www.gamedev.net/topic/442138-packing-a-float-into-a-a8r8g8b8-texture-shader/
*/

float unpackToFloat(vec4 value){

	const vec4 bitSh = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

vec4 packToVec4(float value){

   const vec4 bitSh = vec4(256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0);
   const vec4 bitMsk = vec4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);
   vec4 res = value * bitSh;
   res = fract(res);
   res -= res.xxyz * bitMsk;

   return res;
}

vec3 packToVec3(float value){

   const vec3 bitSh = vec3(256.0 * 256.0, 256.0, 1.0);
   const vec3 bitMsk = vec3(0.0, 1.0 / 256.0, 1.0 / 256.0);
   vec3 res = fract(value * bitSh);
   res -= res.xxy * bitMsk;

   return res;
}

vec2 packToVec2(float value){

   const vec2 bitSh = vec2(256.0, 1.0);
   const vec2 bitMsk = vec2(0.0, 1.0 / 256.0);
   vec2 res = fract(value * bitSh);
   res -= res.xx * bitMsk;


   return res;
}

void main(){

    //if (gl_FragCoord.y>550)
    //    discard;

    //float scal= -1.0 * pSize/screenX;
    //zPos+=abs(gl_FragCoord.x/screenX-(coord.x- pSize/2.0 )  ) * scal;
    //zPos+=abs(gl_FragCoord.y/screenY-coord.y-pSize/2.0)*scal;

   // if (abs(gl_FragCoord.y/screenY-coord.y)>0.01)
    //    discard;
    //if (abs(gl_FragCoord.x/screenX-coord.x)>0.01)
     //   discard;


    //gl_FragDepth=zPos/farClip;
    vec4 objColor=gl_Color * postColor;

    gl_FragData[0]=objColor;
    gl_FragData[1].xy=packToVec2(zPos);
    gl_FragData[1].zw=packToVec2(oID);

    //fog between 0.25 and 0.3
    //nantucket...
    float minFog=0.25;
    float maxFog=0.35;

    //once out of nantucket...
    //float minFog=0.7;
    //float maxFog=0.85;
    //fog goes from 0 to 1;
    float fog=min((zPos-minFog)*1.0/(maxFog-minFog),maxFog);
    int fogAmountX=int(fog*screenX);
    int fogAmountY=int(fog*screenY);

    if (zPos>minFog){
            //gl_FragData[0].rgb=vec3(fogAmount/100.0);
            //gl_FragData[0].rgb=vec3(gl_FragCoord.x/1280.0);
            //return;
            //every pixel should be discarded when fog==100

            //every 100th pixel should be discarded when fog ==0

            if ( int(gl_FragCoord.x)%int(screenX/fogAmountX)<2 && int(gl_FragCoord.y)%int(screenY/fogAmountY)<2){
              discard;
            }
            else{
                gl_FragData[0].rgb=objColor.rgb+vec3(fog*8.0,fog*0.5,0.0);
            }
    }

/*
    objColor.r=floor(objColor.r*100.0)/100.0 ;
    objColor.r=max(0.0,objColor.r);

   if (!bComputeLight){
        if (objColor.r>0.1){
            objColor.r-=0.004;
        }
        else{
            objColor.r+=0.004;
        }
        objColor.a=1.0;
   }
*/
    //if (gl_FragCoord.z>0.15)
    //gl_FragData[0]=objColor;
    //gl_FragData[0]=objColor;

    //gl_FragData[1] =packToVec4(zPos);


}


