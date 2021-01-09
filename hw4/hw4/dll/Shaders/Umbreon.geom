#version 430 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
    vec2 uv;
    vec4 worldPos;
} gs_in[];

uniform mat4 P;
uniform float tc;
uniform float falling;

out vec3 f_normal;
out vec4 f_worldPos;
out vec2 texCoords;

void triangle(int index)
{
    vec4 drop = vec4(0, -falling*0.2, 0, 0.0f);
    if(falling != 0){
        drop += vec4(gs_in[index].normal, 0.0f);
        drop *= falling*0.1;
    }

    texCoords = gs_in[index].uv;
    f_normal = gs_in[index].normal;
    f_worldPos = gs_in[index].worldPos;
    gl_Position = gl_in[index].gl_Position + drop;
    EmitVertex();
    
    texCoords = gs_in[index+1].uv;
    f_normal = gs_in[index+1].normal;
    f_worldPos = gs_in[index+1].worldPos;
    gl_Position = gl_in[index+1].gl_Position + drop;
    EmitVertex();

    texCoords = gs_in[index+2].uv;
    f_normal = gs_in[index+2].normal;
    f_worldPos = gs_in[index+2].worldPos;
    gl_Position = gl_in[index+2].gl_Position + drop;
    EmitVertex();
    
    EndPrimitive();
}

void main()
{
    for(int i=0;i<gl_in.length();i+=3){
        triangle(i);
    }
}
