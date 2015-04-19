uniform sampler2D tex;


uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;
uniform float farClip;

varying float zPos;
varying float oID;

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
   res -= res.xx * bitMsk;

   return res;
}

void main(){

    vec4 color = texture2D(tex,gl_TexCoord[0].st);
    //vec4 objColor=color * postColor;
    vec4 objColor=postColor;


    objColor.r=floor(objColor.r*100.0)/100.0 ;
    objColor.r=min(1.0,max(0.0,objColor.r));

    objColor.g=floor(objColor.g*100.0)/100.0 ;
    objColor.g=min(1.0,max(0.0,objColor.g));

    objColor.b=floor(objColor.b*100.0)/100.0 ;
    objColor.b=min(1.0,max(0.0,objColor.b));

    objColor.a=1.0;

    //add large amounts of unlitness
    objColor.r+=0.000;

    //add large amounts of specularity
    objColor.g+=0.000;

    gl_FragData[0]=objColor;

//transparency...
    if (objColor.a < 0.01){
        discard;
    }else{
        gl_FragDepth=gl_FragCoord.z;
        gl_FragData[0].a=1.0;
    }
    //gl_FragData[0].a=1.0;

     gl_FragData[1].xy=packToVec2(zPos);
     gl_FragData[1].zw=packToVec2(oID);

}
