#version 150

in vec2 texCoord;

uniform sampler2D render_tex;

layout(location = 0) out vec4 blurredFragment;

void main(){
	blurredFragment = vec4(1.0,0.0,0.0,1.0);
}