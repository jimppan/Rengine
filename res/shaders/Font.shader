#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

out vec2 fragUV;

uniform mat4 P;

void main()
{
	gl_Position = P * vec4(position.xy, 1.0, 1.0);
	fragUV = uv;
};

#shader fragment
#version 330 core

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 fragUV;
out vec4 outColor;

void main()
{
	vec4 texColor = vec4(1.0, 1.0, 1.0, texture(u_Texture, fragUV).r);
	outColor = u_Color * texColor;
}