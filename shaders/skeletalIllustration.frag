uniform sampler2D sceneTex;
uniform float objectID;
uniform float farClip;

uniform vec4 postColor;
uniform bool bComputeLight;

varying float zPos;

float PI = 3.14159265358979323846264;

/*
*   Main
*/


vec4 packToVec4(float value){

   const vec4 bitSh = vec4(256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0);
   const vec4 bitMsk = vec4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);
   vec4 res = fract(value * bitSh);
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
   res -= res.xxy * bitMsk;

   return res;
}

void main(){

    gl_FragData[0]= gl_Color * postColor;
    //gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

       gl_FragData[1].xy=packToVec2(zPos/farClip);
        gl_FragData[1].zw=packToVec2((objectID+100.0)/65536.0);

}

