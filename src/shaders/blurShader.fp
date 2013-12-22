#version 150

in vec2 texCoord;

uniform sampler2D render_tex;
uniform float x;
uniform float y;

layout(location = 0) out vec4 blurredFragment;

void main(){
	float stepx = 1.0/800.0;
	float stepy = 1.0/600.0;
	//blurredFragment = vec4(1.0,0.0,0.0,1.0);
	blurredFragment = texture(render_tex,vec2(texCoord.x-stepx, texCoord.y+stepy));
	blurredFragment += texture(render_tex,vec2(texCoord.x, texCoord.y+stepy));
	blurredFragment += texture(render_tex,vec2(texCoord.x+stepx, texCoord.y+stepy));
	blurredFragment += texture(render_tex,vec2(texCoord.x-stepx, texCoord.y));
	blurredFragment += texture(render_tex,vec2(texCoord.x, texCoord.y));
	blurredFragment += texture(render_tex,vec2(texCoord.x+stepx, texCoord.y));
	blurredFragment += texture(render_tex,vec2(texCoord.x-stepx, texCoord.y-stepy));
	blurredFragment += texture(render_tex,vec2(texCoord.x, texCoord.y-stepy));
	blurredFragment += texture(render_tex,vec2(texCoord.x+stepx, texCoord.y-stepy));
	blurredFragment = blurredFragment/9;
	//blurredFragment += texture(render_tex,vec2(texCoord.x,texCoord.y);
}