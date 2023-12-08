#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 specular;
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
};

uniform Light light;
uniform Material material;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;

uniform vec3 camera_pos;



void main() {

    vec3 frag_norm = normalize(frag_normal); 
    vec3 light_dir = normalize(light.pos - frag_pos);
    float diff = max(dot(frag_norm,light_dir),0);

    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir,frag_norm);
    float spec = pow(max(dot(view_dir,reflect_dir),0.),material.shininess);


    vec3 specular = light.specular *  spec * vec3(texture(material.specular,frag_tex_coords));
    vec3 ambient = light.ambient *  vec3(texture(material.diffuse,frag_tex_coords));
    vec3  diffuse =  diff * light.diffuse * vec3(texture(material.diffuse,frag_tex_coords));


    vec3 result = (diffuse + ambient + specular); 
    FragColor = vec4(result,1.);
}
