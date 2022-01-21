#shader vertex

#version 330 core

layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
layout(location = 1) in vec2 aTexCoord;

//out vec3 OurColor;
out vec2 TexCoord;

uniform mat4 Transform;
//uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Transform * vec4(aPos, 1.0f);
	//OurColor = aColor;
	TexCoord = aTexCoord;
}

#shader fragment

#version 330 core

//in vec3 OurColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D Texture1;
void main()
{
	FragColor = texture(Texture1, TexCoord);
}