#version 150

in vec2 texCoord;

out vec4 FragColor;

uniform bool useHDR;
uniform bool useSSAO;

uniform bool onlySSAO;

uniform float exp;
uniform float bMax;
uniform float bloomStrength;

uniform sampler2D color;
uniform sampler2D bloom;


void main() {
	//float exp = 1.0;
	//float bMax = 1.2;
	//float bloomStrength = 0.8;
	vec4 result = vec4(0.0);
	vec3 bloomedSample = texture(bloom,texCoord*0.5).rgb * bloomStrength;
	float ssao = texture(bloom,texCoord*0.5).a;
	vec4 colorSample = texture(color,texCoord);
	float toneMap = exp * (exp / bMax + 1.0) / (exp + 1.0);
	if(useHDR) {
		FragColor = (texture(color, texCoord) + texture(bloom,texCoord*0.5) * bloomStrength) * toneMap;
	} else {
		FragColor = texture(color, texCoord);
	}
	if(useSSAO){
		if(!useHDR) {
			FragColor = texture(color, texCoord) * ssao;
		} else {
			FragColor = ((texture(color, texCoord) + texture(bloom,texCoord*0.5) * bloomStrength) * toneMap) * ssao;
		}
	} else {
		FragColor = texture(color, texCoord);
	}
	if(onlySSAO) {
		FragColor = vec4(ssao);
	}
}