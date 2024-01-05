#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 daytimeColor;

void main()
{
    FragColor = texture(skybox, TexCoords) * vec4(daytimeColor, 1);
}