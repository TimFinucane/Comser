#version 150

uniform sampler2DArray texture;

in uint fragLayer;
in vec2 fragTexCoord;

void main()
{
	gl_FragColor = texture( texture, fragTexCoord, fragLayer );
}