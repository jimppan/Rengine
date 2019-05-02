#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 fragColor;
out vec2 fragUV;

uniform mat4 P;

void main()
{
	gl_Position = P * vec4(position.xyz, 1.0);
	fragColor = color;
};

#shader fragment
#version 330 core

uniform vec4 u_Color;

in vec4 fragColor;

out vec4 outColor;

void main()
{
	outColor = u_Color * fragColor;
}