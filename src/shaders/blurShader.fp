#version 150

in vec2 texCoord;

uniform sampler2D render_tex;
uniform vec2 res;
uniform int kernelSize;

layout(location = 0) out vec4 blurredFragment;

void main(){
	vec3 tmp = vec3(1.0);
	float stepx = 1.0/res.x;
	float stepy = 1.0/res.y;
	//int kernelSize = 16;
	int start = kernelSize/2;
	blurredFragment = vec4(1.0);
	for(int i=-start; i < start; i++){
		for(int j=-start; j < start; j++) {
			//blurredFragment += texture(render_tex,vec2(texCoord.x+i*stepx, texCoord.y+j*stepy));
			tmp += texture(render_tex,vec2(texCoord.x+i*stepx, texCoord.y+j*stepy)).xyz;
		}
	}
	tmp = tmp/(kernelSize*kernelSize);
	blurredFragment = vec4(tmp,texture(render_tex,texCoord).a);
	//blurredFragment = blurredFragment/(kernelSize*kernelSize);
}