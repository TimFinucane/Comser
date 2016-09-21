#version 320

uniform sampler2D texture;

in vec2 fragTexCoord;

void main()
{
	gl_FragColor = texture( texture, texCoord );
}