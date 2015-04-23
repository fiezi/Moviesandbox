uniform vec4 postColor;
uniform bool bComputeLight;
uniform float specularity;

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

/*
    float scal= -1.0 * pSize/screenX;
    zPos+=abs(gl_FragCoord.x/screenX-(coord.x- pSize/2.0 )  ) * scal;
    zPos+=abs(gl_FragCoord.y/screenY-coord.y-pSize/2.0)*scal;

    if (abs(gl_FragCoord.y/screenY-coord.y)>0.01)
        discard;
    if (abs(gl_FragCoord.x/screenX-coord.x)>0.01)
        discard;
*/

    //gl_FragDepth=zPos/farClip;
    //vec4 objColor=gl_Color * postColor;
    vec4 objColor=postColor;

    objColor.r=(int(objColor.r*1000.0)/10)*0.01 ;
    objColor.r=min(0.90,max(0.0,objColor.r));

    objColor.g=(int(objColor.g*1000.0)/10)*0.01 ;
    objColor.g=min(0.90,max(0.0,objColor.g));

    objColor.b=(int(objColor.b*1000.0)/10)*0.01 ;
    objColor.b=min(0.90,max(0.0,objColor.b));

    if (!bComputeLight)
        objColor.r+=0.005;

    //add large amounts of specularity
    objColor.g+=0.000;

    //add large amounts of normalBlur?
    objColor.b+=0.002;


    gl_FragData[0]=objColor;
    /*
    gl_FragData[0].r=min(1.0, objColor.r);
    gl_FragData[0].g=min(1.0, objColor.r);
    gl_FragData[0].b=min(1.0, objColor.r);
*/
    gl_FragData[1].xy=packToVec2(zPos);
    gl_FragData[1].zw=packToVec2(oID);


}


