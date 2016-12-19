#version 330

uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 colour;

void main()
{
	colour = texture( tex, fragTexCoord );
}