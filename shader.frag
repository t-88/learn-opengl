#version 330 core
out vec4 FragColor;

in vec3 frag_color;
in vec2 tex_coord;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex,tex_coord);// * vec4(frag_color,1.);
}
