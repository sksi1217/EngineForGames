#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexCoord;
layout(location=2)in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 texCoordStart;
uniform vec2 texCoordEnd;
uniform vec2 origin;

out vec2 TexCoord;
out vec4 Color;

void main()
{
    // ! Смещаем вершины так, чтобы origin стал (0, 0) в локальных координатах
    vec3 adjustedPos=aPos+vec3(origin,0.);
    
    // ! Применяем трансформации модели и проекции
    gl_Position=projection*view*model*vec4(adjustedPos,1.);
    
    // ! Передаем текстурные координаты
    TexCoord=texCoordStart+aTexCoord*(texCoordEnd-texCoordStart);
    // ! Передаем цвет
    Color=aColor;
}
