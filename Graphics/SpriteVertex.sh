#version 330

layout(location=0) in vec2 vertexPos;
layout(location=1) in vec2 vertexTex;

layout(location=2) in vec2 position;
layout(location=3) in vec2 scale;
layout(location=4) in uint layer;

flat out uint fragLayer;
out vec2 fragTexCoord;

void main()
{
    fragLayer = layer;
	fragTexCoord = vertexTex;
	gl_Position.xyz = vec3( (position + vertexPos * scale), 0.0 );
}