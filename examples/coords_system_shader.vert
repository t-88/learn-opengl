#version 330 core
layout (location = 0) in vec3 _Pos;
layout (location = 1) in vec2 _TexCoord;

out vec3 frag_color;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
   tex_coord = _TexCoord;
   gl_Position = proj * view * model * vec4(_Pos, 1.0);
}
