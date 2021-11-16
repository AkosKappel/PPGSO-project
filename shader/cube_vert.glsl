#version 330 core
layout (location = 0) in vec3 Position;

out vec3 texCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    vec4 pos = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoords = vec3(Position.x, Position.y, -Position.z);
}