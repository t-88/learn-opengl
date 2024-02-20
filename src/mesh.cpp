#include "mesh.hpp"
#include <string>

Mesh::Mesh() { }

Mesh::Mesh(std::vector<Vertex> _vertices,std::vector<uint32_t> _indices,std::vector<Texture> _textures) {
    vertices = _vertices;
    indices = _indices;
    textures = _textures;

    setup_mesh();
}

Mesh::~Mesh() {
    
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(vertices[0]),&vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(indices[0]),&indices[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,tex_coords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

    
void Mesh::draw(Shader& shader) {
    uint32_t diffuse_nbr = 1;
    uint32_t specular_nbr = 1;

    for (int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string name = textures[i].type; 
        if(name == "texture_diffuse") {
            number = std::to_string(diffuse_nbr++);
        } else if(name == "texture_specular") { 
            number = std::to_string(specular_nbr++);
        }
        glActiveTexture(GL_TEXTURE0 + i);
        shader.set_int((name  + number).c_str(),i);
        glBindTexture(GL_TEXTURE_2D,textures[i].ID);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,&indices[0]);
    glBindVertexArray(0);


    glActiveTexture(GL_TEXTURE0);
}
