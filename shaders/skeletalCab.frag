uniform sampler2D sceneTex;
uniform float objectID;
uniform float farClip;

uniform vec4 postColor;
uniform bool bComputeLight;

varying float zPos;
varying float oID;

float PI = 3.14159265358979323846264;


vec4 packToVec4(float value){

   const vec4 bitSh = vec4(255.0 * 255.0 * 255.0, 255.0 * 255.0, 255.0, 1.0);
   const vec4 bitMsk = vec4(0.0, 1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0);
   vec4 res = fract(value * bitSh);
   res -= res.xxyz * bitMsk;

   return res;
}

vec3 packToVec3(float value){

   const vec3 bitSh = vec3(255.0 * 255.0, 255.0, 1.0);
   const vec3 bitMsk = vec3(0.0, 1.0 / 255.0, 1.0 / 255.0);
   vec3 res = fract(value * bitSh);
   res -= res.xxy * bitMsk;

   return res;
}

vec2 packToVec2(float value){

   const vec2 bitSh = vec2(255.0, 1.0);
   const vec2 bitMsk = vec2(0.0, 1.0 / 255.0);
   vec2 res = fract(value * bitSh);
   res -= res.xx * bitMsk;

   return res;
}

/*
*   Main
*/

void main(){

    gl_FragData[0]= gl_Color * postColor;
    //gl_FragData[0]=vec4(1.0,1.0,1.0,1.0);

    gl_FragData[1].xy=packToVec2(zPos);
    gl_FragData[1].zw=packToVec2(oID);

    if (mod(gl_FragCoord.x,10.0)>5.0 + 2.0*sin(zPos) && mod(gl_FragCoord.y,8.0)>4.0+ 2.0*sin(zPos)){
        gl_FragData[0]*=0.0;
        gl_FragData[0].a=1.0;
        //gl_FragData[0].xyz=gl_FragData[0].xyz * gl_FragData[0].xyz;
    }




}

