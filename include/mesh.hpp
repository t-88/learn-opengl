#pragma once
#include <vector>
#include "pch.hpp"
#include "texture.hpp"
#include "shader.hpp"


struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};



class Mesh {
private:
    uint32_t VAO,VBO,EBO;
    void setup_mesh();
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;


    Mesh(std::vector<Vertex>,std::vector<uint32_t>,std::vector<Texture>);
    ~Mesh();
    void draw(Shader& shader);
};