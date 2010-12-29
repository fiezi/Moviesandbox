uniform sampler2D tex; // rendered scene
uniform sampler2DShadow shadowTex; // rendered shadow texture
uniform sampler2D depthTex; // rendered scene texture
varying vec2 texCoord;

vec2 camerarange;
vec2 screensize;



float readDepth( in vec2 coord ) {
	//return (2.0 * camerarange.x) / (camerarange.y + camerarange.x - texture2D( depthTex, coord ).x * (camerarange.y - camerarange.x));
  return  texture2D( depthTex, coord ).x ;
}


void main(void){
	float depth = readDepth( texCoord );
	float d;

  camerarange=vec2 (0.2,10.0);
  screensize= vec2 (400.0,300.0 );

	float pw = 0.25 / screensize.x;
	float ph = 0.25 / screensize.y;

	float aoCap = 1.2;

	float ao = 0.0;

	float aoMultiplier=30.0;

	float depthTolerance = 0.001;

	d=readDepth( vec2(texCoord.x+pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x+pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;

	d=readDepth( vec2(texCoord.x+pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x+pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;

	d=readDepth( vec2(texCoord.x+pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x+pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;

	d=readDepth( vec2(texCoord.x+pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y+ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x+pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	d=readDepth( vec2(texCoord.x-pw,texCoord.y-ph));
	ao+=min(aoCap,max(0.0,depth-d-depthTolerance) * aoMultiplier);

	ao/=16.0;

	gl_FragColor = vec4(1.0-ao) * texture2D(tex,texCoord);
	gl_FragColor =  texture2D(tex,texCoord) - vec4(ao);
	gl_FragColor.a=1.0;
}
