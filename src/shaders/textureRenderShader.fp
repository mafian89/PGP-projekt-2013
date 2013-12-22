#version 150

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D fTexture;

void main() {
	if(texture(fTexture,texCoord).a <= 0.0) {
		discard;
	}
	FragColor = texture(fTexture,texCoord);
}