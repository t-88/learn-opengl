#include "model.hpp"

Model::Model(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);


    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        ERROR_DY("assimp failure: ",importer.GetErrorString());
    }
    dir = path.substr(0, path.find_last_of('/'));

    process_node(scene);
}


Model::~Model() {
}


void Model::load_model(std::string path){
    
}
void Model::process_node(const aiScene* scene) {
    for (size_t i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        process_mesh(mesh);
    }
}
void Model::process_materials(Mesh* mesh,const aiScene* scene) {
    for (size_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
        std::vector<Texture> diffuse_maps = load_material_texture(material,aiTextureType_DIFFUSE,"texture_diffuse");
        std::vector<Texture> specular_maps = load_material_texture(material,aiTextureType_SPECULAR,"texture_specular");
    
        mesh->textures.insert(mesh->textures.end(),diffuse_maps.begin(),diffuse_maps.end());
        mesh->textures.insert(mesh->textures.end(),specular_maps.begin(),diffuse_maps.end());
    }
    
}
void Model::process_mesh(aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    static int counter = 0;
    printf("loading mesh %d\n",counter++);

    for (size_t j = 0; j < mesh->mNumVertices; j++) {
        Vertex vertex;
        vertex.pos = { 
            mesh->mVertices[j].x,
            mesh->mVertices[j].y,
            mesh->mVertices[j].z
        };

        vertex.normal = { 
            mesh->mNormals[j].x,
            mesh->mNormals[j].y,
            mesh->mNormals[j].z,
        };

        if(mesh->mTextureCoords[0]) {
            vertex.tex_coords = { 
                mesh->mTextureCoords[0][j].x,
                mesh->mTextureCoords[0][j].y,
            };
        } else {
            vertex.tex_coords = { 0.,0. };
        }

        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (size_t k = 0; k < face.mNumIndices; k++) {
                indices.push_back(face.mIndices[k]);
            }
        }
    }
} 


std::vector<Texture> Model::load_material_texture(aiMaterial* mat,aiTextureType type, std::string type_name) {
    std::vector<Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type,i,&str);
        Texture texture((dir + (std::string)"/" + ((std::string)str.C_Str())).c_str());
        texture.type = type_name;
        textures.push_back(texture);
    }
    return textures;
}
void Model::draw(Shader& shader) {
    for (uint32_t i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
    
}