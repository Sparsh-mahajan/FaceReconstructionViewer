#shader vertex

#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Transform * vec4(aPos, 1.0f);
}

#shader fragment

#version 330 core

out vec4 FragColor;
uniform vec4 PointsColor;
void main()
{
	FragColor = PointsColor;
}