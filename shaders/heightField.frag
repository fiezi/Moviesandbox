uniform sampler2D tex;


uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;
uniform float farClip;

varying float zPos;
varying float oID;

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

void main(){

    vec4 objColor=gl_Color * postColor;



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


    gl_FragData[0]=objColor;


    gl_FragData[0].a=1.0;

     gl_FragData[1].xy=packToVec2(zPos);
     gl_FragData[1].zw=packToVec2(oID);

}
