#pragma once
#include <vector>
#include <string>

#include "pch.hpp"
#include "shader.hpp"
#include "mesh.hpp"

class Model
{
private:
    std::vector<Mesh> meshes;
    std::string dir;

    void load_model(std::string path);
    void process_node(const aiScene* scene);
    void process_materials(Mesh* mesh,const aiScene* scene); 
    void process_mesh(aiMesh* mesh); 
    std::vector<Texture> load_material_texture(aiMaterial* mat,aiTextureType type, std::string type_name);



public:
    Model(std::string path);
    ~Model();
    void draw(Shader& shader);
};

