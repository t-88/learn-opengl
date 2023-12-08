#version 330 core
layout (location = 0) in vec3 _Pos;
layout (location = 1) in vec3 _Norm;

out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {

   gl_Position = proj * view * model * vec4(_Pos, 1.0);

   frag_pos = (model * vec4(_Pos,1.)).xyz;
   frag_normal = mat3(transpose(inverse(model))) * _Norm;

}
