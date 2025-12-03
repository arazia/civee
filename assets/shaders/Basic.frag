#version 330 core

// layout(location = 0) out vec4 o_Color;

// uniform vec4 u_Color;
// in vec4 v_Color;
// in vec2 v_TexCoord;

in vec3 v_Normal;
in vec3 v_WorldPos;
in vec2 v_TexCoord;
in vec4 v_Color;

out vec4 o_Color;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};


uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;

#define MAX_POINT_LIGHTS 16
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    if (texColor.a < 0.1) discard;

    vec3 normal = normalize(v_Normal);

    vec3 ambient = vec3(0.2);

    vec3 lightDir = normalize(-u_DirectionalLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 sunInfo = diff * u_DirectionalLight.color;

    vec3 pointInfo = vec3(0.0);
    for (int i = 0; i < u_PointLightCount; i++) {
        vec3 lightVec = u_PointLights[i].position - v_WorldPos;
        float dist = length(lightVec);
        
        if (dist < u_PointLights[i].radius) {
            // Attenuation (Fade out)
            // Simple linear fade: 1 at center, 0 at radius
            float atten = 1.0 - (dist / u_PointLights[i].radius);
            atten = max(atten, 0.0); 
            // Or use quadratic: atten = 1.0 / (1.0 + 0.1*dist + 0.01*dist*dist);
            
            vec3 pDir = normalize(lightVec);
            float pDiff = max(dot(normal, pDir), 0.0);
            
            pointInfo += pDiff * u_PointLights[i].color * u_PointLights[i].intensity * atten;
        }
    }

    vec3 finalLight = ambient + sunInfo + pointInfo;
    
    o_Color = vec4(texColor.rgb * finalLight, texColor.a) * v_Color;
}
