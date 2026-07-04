#version 330 core

in vec2 fragUV;
in vec4 fragColor;
in float fragArrayLayer;

uniform sampler2DArray spriteTexArray;

out vec4 outColor;


void main() {
	vec4 texColor = texture(spriteTexArray, vec3(fragUV, fragArrayLayer));
	outColor = texColor * fragColor;
}