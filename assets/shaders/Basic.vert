#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in mat4 a_InstanceMatrix;

uniform mat4 u_ViewProjection;
uniform float u_DepthBias;

out vec3 v_Normal;
out vec3 v_WorldPos;
out vec4 v_Color;
out vec2 v_TexCoord;

void main() {
    vec4 world_pos = a_InstanceMatrix * vec4(a_Position, 1.0);
    v_WorldPos = vec3(world_pos);

    gl_Position = u_ViewProjection * world_pos;
    gl_Position.z -= u_DepthBias * gl_Position.w;

    v_Normal = mat3(a_InstanceMatrix) * a_Normal;
    
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
}
