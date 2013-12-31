#version 150

//SSAO code from http://www.gamerendering.com/2009/01/14/ssao/

in vec2 texCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D render_tex;
uniform sampler2D normal_tex;
uniform sampler2D random_tex;
uniform float treshold;

uniform float totStrength;
uniform float strength;
uniform float offset;
uniform float falloff;
uniform float rad;

#define SAMPLES 10
const float invSamples = 1.0/10.0;

float computeSSAO(sampler2D normal_tex, sampler2D random_tex) {
	// Nahode vektory na jednotkovy kruznici 
	const vec3 pSphere[10] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),vec3(-0.06533369, 0.3647007, -0.13746321),vec3(-0.6539235, -0.016726388, -0.53000957),vec3(0.40958285, 0.0052428036, -0.5591124),vec3(-0.1465366, 0.09899267, 0.15571679),vec3(-0.44122112, -0.5458797, 0.04912532),vec3(0.03755566, -0.10961345, -0.33040273),vec3(0.019100213, 0.29652783, 0.066237666),vec3(0.8765323, 0.011236004, 0.28265962),vec3(0.29264435, -0.40794238, 0.15964167));
	// Normala z nahodne textury
	vec3 fres = normalize((texture(random_tex,texCoord*offset).xyz*2.0) - vec3(1.0));
	// Prectu aktualni normalu
	vec4 currentPixelSample = texture(normal_tex,texCoord);
	// A dostanu hloubku
	float currentPixelDepth = currentPixelSample.a;
 
	// Koordinaty fragmentu ve screen space
	vec3 frag_coords = vec3(texCoord.xy,currentPixelDepth);
	// Normala aktualniho fragmentu
	vec3 norm = currentPixelSample.xyz;
 
	float tmp = 0.0;
	// adjust for the depth
	float radD = rad/currentPixelDepth;
 
	vec3 ray, se, occNorm;
	float occluderDepth, depthDifference, normDiff;
 
	for(int i=0; i<SAMPLES;++i)
	{
		// get a vector (randomized inside of a sphere with radius 1.0) from a texture and reflect it
		ray = radD*reflect(pSphere[i],fres);
 
		// if the ray is outside the hemisphere then change direction
		se = frag_coords + sign(dot(ray,norm) )*ray;
 
		// get the depth of the occluder fragment
		vec4 occluderFragment = texture(normal_tex,se.xy);
 
		// get the normal of the occluder fragment
		occNorm = occluderFragment.xyz;
 
		// if depthDifference is negative = occluder is behind current fragment
		depthDifference = currentPixelDepth-occluderFragment.a;
 
		// calculate the difference between the normals as a weight
		normDiff = (1.0-dot(occNorm,norm));

		// the falloff equation, starts at falloff and is kind of 1/x^2 falling
		tmp += step(falloff,depthDifference)*normDiff*(1.0-smoothstep(falloff,strength,depthDifference));
	}
 
	// Vysledek
	return 1.0-totStrength*tmp*invSamples;
}

void main() {
	vec4 color = texture(render_tex,texCoord);
	if( dot(vec3(0.30, 0.59, 0.11),color.rgb) > treshold) {
		FragColor = vec4(color.rgb,1.0);
	} else {
		//FragColor = vec4(0.0,0.0,0.0,1.0);
	}
	FragColor.a = computeSSAO(normal_tex, random_tex);
	//FragColor = vec4(1.0);
}