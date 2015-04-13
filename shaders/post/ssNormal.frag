uniform float time;
uniform float screensize;
uniform float shadow_size;
uniform float lighting_size;
uniform float screenX;
uniform float screenY;

uniform float nearClip;
uniform float farClip;

uniform sampler2D tex; // rendered scene 512*512 (lighting resolution)
uniform sampler2D depthTex; // rendered normals and depth texture
uniform sampler2D shadowTex; // rendered shadow textures - 256x256 (shadow resolution)

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 cameraMatrix;
uniform mat4 cameraInverse;
uniform mat4 inverseCameraRotationMatrix;


uniform vec3 camLoc;
uniform vec3 camX;
uniform vec3 camY;
uniform vec3 camZ;


varying vec2 texCoord;

//light position stuff

varying vec3 lightColor;
varying vec4 lightPos;
varying float lightZScreen;
varying mat4 lightSpaceMat;

const float specularExp = 32.0;

//pixel position stuff
vec4 pixelPos;
float zPos;
float zPosScreen;
float oID;

// blur variables


    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;

float unpackToFloat(vec4 value){

	const vec4 bitSh = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

float unpackToFloat(vec3 value){

	const vec3 bitSh = vec3(1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

float unpackToFloat(vec2 value){

	const vec2 bitSh = vec2(1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}


vec4 blur5(sampler2D myTex,vec2 tc, float bias){

      float spread=1.0/screenX;

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
        sample[i]=texture2D(myTex , tc + tc_offset[i], bias);
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


vec4 blur3(sampler2D myTex, vec2 tc, float bias){

      vec4 sample[9];

      float spread=1.0/screenX;//  * unpackToFloat(texture2D(myTex , tc).rg)*farClip/16.0;
      //float spread=0.250/shadow_size;//   * texture2D(myTex , tc).a/32.0;

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
        sample[i]=texture2D(myTex , tc + tc_offset[i], bias);
      }

      vec4 blurredColor=(
                         sample[0] + (2.0* sample[1]) + sample[2] +
                         (2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
                         sample[6] + (2.0* sample[7]) + sample[8]
                        )/ 13.0;
      //blurredColor.a=1.0;
      return(blurredColor);
}



///pixelPosition in eyeSpace ( pixel on ground plane stays the same independent of camera's distance to ground plane )
void getPixelLoc(){

    vec2 tc=texCoord;
    //TODO: how can we make the lookup dependent on drawing/no drawing
    zPos= unpackToFloat(texture2D(depthTex,tc,0.0).rg) * (farClip);


    //zPos= unpackToFloat(blur3(depthTex,tc,(farClip/zPos)*0.01 ).rg) * (farClip);
    //zPos= unpackToFloat(blur3(depthTex,tc,1.0 ).rg) * (farClip);
    //zPos= unpackToFloat(texture2D(depthTex,tc,(farClip/zPos)*0.01 ).rg) * (farClip);
    //zPos= unpackToFloat(blur5(depthTex,tc ).rg) * (farClip);
    //zPosScreen=farClip/ (farClip - zPos * (farClip- nearClip));
    //zPos=1.0/zPosScreen;


}



vec4 computeNormals(){


    //normal in Eye Space is the difference in z
    //we exagerrate along z - because differences become more significant with higher z
    //float dx= dFdx(zPos)/(zPos*zPos);
    //float dy= dFdy(zPos)/(zPos*zPos);
    float dy= dFdy(zPos);
    float dx= dFdx(zPos);
    //float dy= dFdy(zPos );
    //float dx= dFdx(zPos );
    //return vec4 (dx,dy,0.0,1.0) * 1.0;
    //return vec4 (fwidth(zPos)) * 1.0;
    //return vec4 (dy) * 10.0;

    //vec3 pixelNormal=normalize(vec3(dx,dy,fwidth(zPos)* 1.0));
    vec3 pixelNormal=normalize(vec3(sin(dx * PI),sin(dy * PI),0.0));

    pixelNormal.x=dx;
    pixelNormal.y=dy;
    pixelNormal.z=0.0;

     vec3 yVec=   vec3(0,1.0-dy,0);
     vec3 xVec=  vec3(1.0-dx,0,0) ;
     yVec= dy * camY;
     xVec= dx * camX;

    pixelNormal=  cross(yVec,xVec)   ;
    //pixelNormal.x=sin(dx * PI);
    //pixelNormal.y=sin(dy * PI);
    pixelNormal.x=dx;
    pixelNormal.y=dy;
    pixelNormal.z=-(zPos/1024.0);
    //pixelNormal.z=-(zPos/32.0);
    pixelNormal=normalize(pixelNormal);

    pixelNormal=-pixelNormal.x * camX + pixelNormal.y  *camY + pixelNormal.z * camZ;
    pixelNormal*=0.5;
    pixelNormal+=0.5;

    return (vec4(pixelNormal.xyz,1.0));
}




void main(){


      for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);

    getPixelLoc();

    gl_FragColor=computeNormals();
    //    gl_FragColor=vec4(1.0);
    gl_FragColor.a=1.0;

}
