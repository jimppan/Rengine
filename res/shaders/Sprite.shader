#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

out vec4 fragColor;
out vec2 fragUV;

uniform mat4 P;

void main()
{
	gl_Position = P * vec4(position.xyz, 1.0);
	fragColor = color;
	fragUV = uv;
};

#shader fragment
#version 330 core

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec2 u_UVOffset;

in vec4 fragColor;
in vec2 fragUV;

out vec4 outColor;

void main()
{
	vec4 texColor = texture(u_Texture, fragUV + u_UVOffset);
	outColor = u_Color * texColor * fragColor;
}