#version 430

layout(location = 0) in vec3 in_position;

uniform mat4 M, V,P;

void main() {
	gl_Position = P * V * M * vec4(in_position, 1.0);
}
