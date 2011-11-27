uniform sampler2D tex;
uniform vec4 postColor;
uniform bool bComputeLight;
uniform float objectID;
uniform float farClip;

varying vec3 N;
varying float backSide;

varying float zPos;
varying vec4 picking;

float unpackToFloat(vec4 value){

	const vec4 bitSh = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

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

    vec4 color = texture2D(tex,gl_TexCoord[0].st);

    //black if not facing light
    gl_FragData[0]=color * gl_Color;//* postColor;

    if (gl_FragData[0].a<0.5)
        gl_FragData[0].r=1.0;

    //transparency...
    if (gl_FragData[0].a < 0.1){
        discard;
    }
    else{
        gl_FragDepth=gl_FragCoord.z;
        //gl_FragData[1]=vec4(N.x ,N.y ,N.z, zPos );
        //gl_FragData[1]=vec4(dFdx(gl_FragCoord.z)*65536.0 ,dFdy(gl_FragCoord.z)*65536.0 , 0.0,zPos);
    }

     gl_FragData[1].xy=packToVec2(zPos/farClip);
     gl_FragData[1].zw=packToVec2((objectID+100.0)/65536.0);
     //gl_FragData[1]=vec4(zPos/255.0 ,mod(zPos,255.0),objectID, 0.0 );
   //gl_FragData[1]=vec4( zPos, objectID, 0.0, 0.0 );
}
