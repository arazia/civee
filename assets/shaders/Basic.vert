#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

layout(location = 3) in mat4 a_InstanceMatrix;
// layout(location = 6) in vec4 a_InstanceColor;

uniform mat4 u_ViewProjection;
uniform float u_DepthBias;

out vec4 v_Color;
out vec2 v_TexCoord;

void main() {
    vec4 pos = u_ViewProjection * a_InstanceMatrix * vec4(a_Position, 1.0);
    pos.z -= u_DepthBias;
    gl_Position = pos;
    
    // v_Color = a_Color * a_InstanceColor;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
}
