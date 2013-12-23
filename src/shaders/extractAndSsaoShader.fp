#version 150

in vec2 texCoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D render_tex;
uniform float treshold;

void main() {
	vec4 color = texture(render_tex,texCoord);
	if( dot(vec3(0.30, 0.59, 0.11),color.rgb) > treshold) {
		FragColor = vec4(color.rgb,1.0);
	}
	//FragColor = vec4(1.0);
}