#version 150

in vec2 texCoord;

out vec4 FragColor;

uniform bool useHDR;
uniform float exp;
uniform float bMax;
uniform float bloomStrength;

uniform sampler2D color;
uniform sampler2D bloom;


void main() {
	//float exp = 1.0;
	//float bMax = 1.2;
	//float bloomStrength = 0.8;
	vec4 bloomedSample = texture(bloom,texCoord*0.5) * bloomStrength;
	vec4 colorSample = texture(color,texCoord);
	float toneMap = exp * (exp / bMax + 1.0) / (exp + 1.0);
	if(useHDR) {
		FragColor = colorSample + bloomedSample*toneMap;
	} else {
		FragColor = colorSample;
	}
}