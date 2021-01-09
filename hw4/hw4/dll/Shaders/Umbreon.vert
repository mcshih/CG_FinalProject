#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 M, V, P;

// out vec2 uv;

out VS_OUT {
    vec3 normal;
    vec2 uv;
    vec4 worldPos;
} vs_out;

void main() {
  gl_Position = P * V * M * vec4(in_position, 1.0f);
  vs_out.normal = normalize((transpose(inverse(M)) * vec4(in_normal, 1.0)).xyz);
  vs_out.uv= texcoord;
  vs_out.worldPos = vec4(M * vec4(in_position, 1.0f));
}
