#version 330 core

uniform sampler2D Tex;
uniform float lightnessValue;

in vec2 UV;
out vec4 color;

void main()
{
	vec4 lightnessVector = vec4(lightnessValue, lightnessValue, lightnessValue, 1.0);
	//vec4 boolVec = lessThan(texture(Tex, UV.st), Vec4())
	color = lightnessVector * texture(Tex, UV.st);
}