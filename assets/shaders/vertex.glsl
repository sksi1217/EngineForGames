#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 texCoordStart;
uniform vec2 texCoordEnd;

out vec2 TexCoord;

void main()
{
    TexCoord=texCoordStart+aTexCoord*(texCoordEnd-texCoordStart);
    gl_Position=projection*model*vec4(aPos,1.);
}
