#version 330 core
layout(location=0)in vec2 aPos;
layout(location=1)in vec3 aColor;

out vec3 fColor;

uniform mat4 u_ViewProjection;

void main()
{
	fColor=aColor;
	gl_Position=u_ViewProjection*vec4(aPos,0.,1.);
}
