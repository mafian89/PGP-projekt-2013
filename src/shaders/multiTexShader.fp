#version 150

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D color;
uniform sampler2D bloom;


void main() {
	float exp = 1.0;
	float bMax = 1.2;
	vec4 bloomedSample = texture(bloom,texCoord*0.5) * 0.5;
	vec4 colorSample = texture(color,texCoord);
	float toneMap = exp * (exp / bMax + 1.0) / (exp + 1.0);

	FragColor = colorSample + bloomedSample*toneMap;
}