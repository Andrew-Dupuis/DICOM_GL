#version 300 es

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec2 uvFrag;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) {
	gl_Position = projectionMatrix * viewMatrix * vec4(pos,1);
	uvFrag = uv;
};