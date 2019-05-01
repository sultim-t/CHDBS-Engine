#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_CameraSpace;
uniform mat4 u_Model;
uniform mat4 u_LightSpace;

void main()
{
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.Normal = normalize(vec3(u_Model * vec4(aNormal, 0.0)));
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = u_LightSpace * vec4(vs_out.FragPos, 1.0);
    gl_Position = u_CameraSpace * u_Model * vec4(aPos, 1.0);
}