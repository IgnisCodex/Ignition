// Default 2D Shader
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TextureCoordinates;
layout(location = 3) in float a_TextureIndex;

uniform mat4 u_ViewProjection;

out vec4 v_Colour;
out vec2 v_TextureCoordinates;
out float v_TextureIndex;

void main() {
	v_Colour = a_Colour;
	v_TextureCoordinates = a_TextureCoordinates;
	v_TextureIndex = a_TextureIndex;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[32];

in vec4 v_Colour;
in vec2 v_TextureCoordinates;
in float v_TextureIndex;

void main() {
	color = texture(u_Textures[int(v_TextureIndex)], v_TextureCoordinates) * v_Colour;
	// color = v_Colour;
}