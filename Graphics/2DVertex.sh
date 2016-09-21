#version 320

uniform vec2 position;
uniform mat2 matrix;

in vec2 vertex;
in vec2 texCoord;
out vec2 fragTexCoord;

void main()
{
	fragTexCoord = texCoord;
	gl_Position = position + matrix * vertex;
}