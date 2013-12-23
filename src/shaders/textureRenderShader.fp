#version 150

//IN texture coordinates
in vec2 texCoord;

//OUT fragment color
out vec4 FragColor;

//Texture sampler
uniform sampler2D fTexture;
uniform sampler2D bloomTex;

void main() {
	//Discard fragement, which has alpha value lower or equal to 0.0
	if(texture(fTexture,texCoord).a <= 0.0) {
		discard;
	}
	
	float exposure = 0.5;
	float brightMax = 0.98;
	vec4 bloom = texture(bloomTex,texCoord) * 0.5;
	vec4 res = texture(fTexture,texCoord);
	float YD = exposure * (exposure/brightMax + 1.0) / (exposure + 1.0);
	FragColor = (res + bloom*YD);
	
	//Rocket science here
	//FragColor = texture(fTexture,texCoord);
}