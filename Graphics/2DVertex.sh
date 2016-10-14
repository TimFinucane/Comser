#version 150

uniform dvec2 position;
uniform mat2 matrix;

in  dvec2 vertex;
in  dvec2 texCoord;
out dvec2 fragTexCoord;

void main()
{
	fragTexCoord = texCoord;
	gl_Position = position + matrix * vertex;
}