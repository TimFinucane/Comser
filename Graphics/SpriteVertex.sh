#version 330

layout(location=0) in vec2 vertexPos;
layout(location=1) in vec2 vertexTex;

layout(location=2) in vec2  position;
layout(location=3) in vec2  scale;
layout(location=4) in float angle;
layout(location=5) in uint  layer;

uniform mat3 modelProj;

flat out uint fragLayer;
out vec2 fragTexCoord;

void main()
{
    fragLayer = layer;
	fragTexCoord = vertexTex;

	vec2 vert;
	vert.x = vertexPos.x * cos( angle ) - vertexPos.y * sin( angle );
	vert.y = vertexPos.x * sin( angle ) + vertexPos.y * cos( angle );

	gl_Position.xyz = modelProj * vec3( (position + vert * scale), 0.0 );
}