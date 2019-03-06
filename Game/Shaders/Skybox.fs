#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube t_cube0;

void main()
{    
    FragColor = texture(t_cube0, TexCoords);
}