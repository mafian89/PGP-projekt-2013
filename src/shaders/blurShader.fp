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
	
	float tmpA = 1.0;
	int tmp_kernel = 8;
	for(int i=-(tmp_kernel/2); i < (tmp_kernel/2); i++){
		for(int j=-(tmp_kernel/2); j < (tmp_kernel/2); j++) {
			//blurredFragment += texture(render_tex,vec2(texCoord.x+i*stepx, texCoord.y+j*stepy));
			tmpA += texture(render_tex,vec2(texCoord.x+i*stepx, texCoord.y+j*stepy)).a;
		}
	}
	tmpA = tmpA/(tmp_kernel*tmp_kernel);

	
	blurredFragment = vec4(tmp,tmpA);
	//blurredFragment = blurredFragment/(kernelSize*kernelSize);
}