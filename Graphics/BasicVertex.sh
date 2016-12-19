#version 330

layout(location=0) in vec2 vertexPos;
layout(location=1) in vec2 vertexTex;

uniform mat4 modelProj;

out vec2 fragTexCoord;

void main()
{
	fragTexCoord = vertexTex;
	gl_Position = (modelProj * vec4( vertexPos, 0.0, 1.0 ));
}