#version 330 core
layout (location = 0) in vec3 _Pos;

out vec3 frag_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
   gl_Position = proj * view * model * vec4(_Pos, 1.0);
}
