#version 300 es

layout(location=0) in vec4 pos;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 cameraPosition;

out vec3 eyeDirection;
flat out vec3 eyePosition;

void main(void) {
	vec3 volume_translation = vec3(-0.5);
	gl_Position = projectionMatrix * viewMatrix * vec4(pos.xyz + volume_translation, 1);
        eyePosition = (cameraPosition.xyz - volume_translation);
	eyeDirection = pos.xyz - eyePosition;
};