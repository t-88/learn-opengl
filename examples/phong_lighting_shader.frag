#version 330 core
out vec4 FragColor;

uniform vec3 obj_color;
uniform vec3 light_color; 

in vec3 frag_pos;
in vec3 frag_normal;

uniform vec3 light_pos;
uniform vec3 camera_pos;

void main() {


    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;

    vec3 frag_norm = normalize(frag_normal); 
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(frag_norm,light_dir),0);
    vec3  diffuse = diff * light_color;

    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir,frag_norm);
    float specStrength = 0.1;
    float spec = pow(max(dot(view_dir,reflect_dir),0.),32);
    vec3 specular = specStrength * spec * light_color;


    vec3 result = (diffuse + ambient + specular) * obj_color; 
    FragColor = vec4(result,1.);
}
