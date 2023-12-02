#version 330 core
layout (location = 0) in vec3 _Pos;
layout (location = 1) in vec3 _Color;
layout (location = 2) in vec2 _TexCoord;

out vec3 frag_color;
out vec2 tex_coord;

void main() {
   frag_color = _Color;
   tex_coord = _TexCoord;
   gl_Position = vec4(_Pos, 1.0);
}
