#version 330
out vec4 FragColor;

in vec3 texCoords;
uniform samplerCube skyboxTexture;

void main()
{
    FragColor = texture(skyboxTexture, texCoords);
}