attribute float vertexID;
uniform float screenX;
uniform float screenY;
uniform float farClip;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float fov;

varying float zPos;
varying float oID;

float PI = 3.14159265358979323846264;

/*
*   Simplex Noise
*/
vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}


float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}


/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w ;
  //return particleScale;
  //particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  //particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
  if (gl_Position.z>0.3){
      if (gl_Position.z<1.0 )
        return ( (particleScale * 1000.0  ) / (gl_Position.z) );

      return ( (particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );
  }else
  return 1.0;

}

/*
*   Main
*/

void main(){

    gl_FrontColor=gl_Color;

    //reset gl_Vertex coordinate or we create weird distortions!
    vec4 myVertex=gl_Vertex;
    myVertex.w=1.0;


    //Ocean vertices
    //myVertex.z+=1.0*sin(30.0*myVertex.y+time*0.0015) *snoise(vec2(10.1*myVertex.y+time*0.001,1.1*myVertex.y+time*0.001));
    //myVertex.x+=0.02*sin(2.0*myVertex.y*20.0+time*0.002);
    //myVertex.y+=0.01*sin(5.0*myVertex.x*2.0+time*0.0002);
    //myVertex.y+=0.015 * snoise(vec2(5.1*myVertex.x+time*0.0001,2.1*myVertex.y+time*0.0001));
    //myVertex.x+=0.25*sin(30.0*myVertex.y+time*0.0015);// *snoise(vec2(10.1*myVertex.y+time*0.001,1.1*myVertex.y+time*0.001));
    //nice ocean
    myVertex.z=0.5 * sin (120.0*myVertex.x + time*0.0015);
    myVertex.z+=0.5*sin(60.0*myVertex.y+time*0.0005);
    //noisy ocean
    //myVertex.z+=0.1*snoise(vec2(10.0*myVertex.y+time*0.0006,10.0*myVertex.y+time*0.0006));
    //myVertex.z+=0.1*snoise(vec2(40.0*myVertex.x+time*0.0005,40.0*myVertex.x+time*0.0005));
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    myVertex.z+=10.0*cos(0.50*PI*myVertex.x);
    myVertex.z+=10.0*cos(0.50*PI*myVertex.y);


/*
    myVertex.z+=0.5*sin(10.0*myVertex.x+time*0.0015);
    //myVertex.x+=0.02*sin(1.0*myVertex.y*20.0+time*0.001);
    //myVertex.y+=0.1*sin(5.0*myVertex.x*2.0+time*0.0002);
    myVertex.z+=0.05*sin(20.0*myVertex.y+time*0.0005);
*/
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

                                                    //this here is an approximation
    gl_PointSize= pointSize() * (45.0*45.0)/(fov*fov);

    //testing for ocean


    zPos=gl_Position.z/farClip;
    oID= (objectID+100.0) /2048.0;

}




