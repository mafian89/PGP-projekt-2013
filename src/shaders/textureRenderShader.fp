#version 150

//IN texture coordinates
in vec2 texCoord;

//OUT fragment color
out vec4 FragColor;

//Texture sampler
uniform sampler2D fTexture;
uniform sampler2D bloomTex;

uniform bool useHDR;

void main() {
	//Discard fragement, which has alpha value lower or equal to 0.0
	//if(texture(fTexture,texCoord).a <= 0.0) {
	//	discard;
	//}
	
	float exposure = 1.0;
	float brightMax = 1.2;
	if(useHDR) {
		vec4 bloom = texture(bloomTex,texCoord) * 0.5;
		vec4 res = texture(fTexture,texCoord);
		//float Y = dot(vec3(0.30, 0.59, 0.11), res.xyz);
		float YD = exposure * (exposure/brightMax + 1.0) / (exposure + 1.0);
		FragColor = (res + bloom*YD);
	} else {
		//Rocket science here
		FragColor = texture(fTexture,texCoord);
	}
}