#version 150

layout(location=0) in vec2 vertexPos;
layout(location=1) in vec2 vertexTex;

layout(location=2) in vec2 position;
layout(location=3) in vec2 scale;
layout(location=4) in uint layer;

out uint  fragLayer;
out dvec2 fragTexCoord;

void main()
{
    fragLayer = layer;
	fragTexCoord = vertexTex;
	gl_Position = position + vertexPos * scale;
}