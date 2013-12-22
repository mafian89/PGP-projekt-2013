#version 150

//IN texture coordinates
in vec2 texCoord;

//OUT fragment color
out vec4 FragColor;

//Texture sampler
uniform sampler2D fTexture;

void main() {
	//Discard fragement, which has alpha value lower or equal to 0.0
	if(texture(fTexture,texCoord).a <= 0.0) {
		discard;
	}
	//Rocket science here
	FragColor = texture(fTexture,texCoord);
}