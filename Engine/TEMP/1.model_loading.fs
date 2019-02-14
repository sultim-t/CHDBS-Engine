#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D t_diff0;

void main()
{    
    FragColor = texture(t_diff0, TexCoords);
}