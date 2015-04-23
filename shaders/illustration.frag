uniform vec4 postColor;
uniform float objectID;
uniform bool bComputeLight;

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


/*
*   Main
*/

void main(){

    vec4 objColor=gl_Color * postColor;
    //vec4 objColor=postColor;

    objColor.r=(int(objColor.r*1000.0)/10)*0.01 ;
    objColor.r=min(0.99,max(0.0,objColor.r));

    objColor.g=(int(objColor.g*1000.0)/10)*0.01 ;
    objColor.g=min(0.99,max(0.0,objColor.g));

    objColor.b=(int(objColor.b*1000.0)/10)*0.01 ;
    objColor.b=min(0.99,max(0.0,objColor.b));

    if (!bComputeLight)
        objColor.r+=0.005;


    //add large amounts of specularity
    objColor.g+=0.001;//*specularity;

    //add large amounts of normal blur
    objColor.b+=0.002;


    gl_FragData[0]=objColor;
    gl_FragData[1].xy=packToVec2(zPos);
    gl_FragData[1].zw=packToVec2(oID);

}


