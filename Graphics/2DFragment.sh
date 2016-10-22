#version 330

uniform sampler2DArray tex;

flat in uint fragLayer;
in vec2 fragTexCoord;

out vec4 colour;

void main()
{
	colour = texture( tex, vec3( fragTexCoord, fragLayer ) );
}