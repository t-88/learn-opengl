#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 specular;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;

    float quadratic;
    float linear;
    float constant;

    float cutOff;
    float outer_cutOff;
    
    vec3 direction;
};

uniform Light light;
uniform Material material;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;

uniform vec3 camera_pos;



void main() {
    vec3 frag_norm = normalize(frag_normal); 
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(frag_norm,light_dir),0);

    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir,frag_norm);
    float spec = pow(max(dot(view_dir,reflect_dir),0.),material.shininess);


    vec3 specular = light.specular *  spec * vec3(texture(material.specular,frag_tex_coords));
    vec3 ambient = light.ambient *  vec3(texture(material.diffuse,frag_tex_coords));
    vec3 diffuse =  diff * light.diffuse * vec3(texture(material.diffuse,frag_tex_coords));


    float light_dis = length(frag_pos - light.position);
    float attenuation = 1. / (light.constant + light_dis * light.linear + (light_dis * light_dis) *  light.quadratic);
    diffuse *=  attenuation;
    specular *= attenuation;
    ambient *=  attenuation;

    // spot light
    float theta = dot(light_dir,normalize(-light.direction));
    float intensity = clamp((theta - light.outer_cutOff) / (light.cutOff - light.outer_cutOff),0.,1.);
    diffuse *= intensity;
    specular *= intensity;

    

    vec3 result = (diffuse + ambient + specular);
    FragColor = vec4(result,1.);
}
