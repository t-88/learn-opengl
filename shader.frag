#version 330 core
out vec4 FragColor;

in vec3 frag_color;


void main() {
    FragColor = vec4(frag_color,1.);//vec4(1.0f, 0.5f, 0.2f, 1.0f);
};  
