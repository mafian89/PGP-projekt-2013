#version 150

in vec2 texCoord;

out vec4 FragColor;

uniform bool useHDR;
uniform bool useSSAO;

uniform bool onlySSAO;

uniform float scale;

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
	vec3 bloomedSample = texture(bloom,texCoord*scale).rgb * bloomStrength;
	float ssao = texture(bloom,texCoord*scale).a;
	vec4 colorSample = texture(color,texCoord);
	float toneMap = exp * (exp / bMax + 1.0) / (exp + 1.0);
	if(useHDR && useSSAO) {
        FragColor = ((texture(color, texCoord) + vec4(bloomedSample,1.0)) * toneMap) * ssao;
    } else if(useSSAO && !useHDR) {
        FragColor = texture(color, texCoord) * ssao;
    } else if(!useSSAO && useHDR) {
        FragColor = ((texture(color, texCoord) + vec4(bloomedSample,1.0)) * toneMap);
    } else {
        FragColor = texture(color, texCoord);
    }
    if(onlySSAO) {
        FragColor = vec4(ssao);
    }
}