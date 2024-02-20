#include "model.hpp"


static std::vector<Texture> loaded_textures;

Model::~Model() {}
Model::Model(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        ERROR_DY("assimp failure: ",importer.GetErrorString());
    }

    dir = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode,scene);
}

void Model::process_node(aiNode *node,const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh,scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i],scene);
    }
}


Mesh Model::process_mesh(aiMesh *ai_mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    for (size_t i = 0; i < ai_mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.pos.x = ai_mesh->mVertices[i].x;
        vertex.pos.y = ai_mesh->mVertices[i].y;
        vertex.pos.z = ai_mesh->mVertices[i].z;

        vertex.normal.x = ai_mesh->mNormals[i].x;
        vertex.normal.y = ai_mesh->mNormals[i].y;
        vertex.normal.z = ai_mesh->mNormals[i].z;


        if(ai_mesh->mTextureCoords[0]) {
            vertex.tex_coords.x = ai_mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertex.tex_coords.x = 0;
            vertex.tex_coords.y = 0;
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < ai_mesh->mNumFaces; i++) {
        aiFace face = ai_mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(ai_mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

        std::vector<Texture> diffuse_map = load_material_texture(material,aiTextureType_DIFFUSE,"texture_diffuse");
        textures.insert(textures.end(),diffuse_map.begin(),diffuse_map.end());

        std::vector<Texture> specular_map = load_material_texture(material,aiTextureType_SPECULAR,"texture_specular");
        textures.insert(textures.end(),specular_map.begin(),specular_map.end());
    }

    return Mesh(vertices,indices,textures);
}



std::vector<Texture> Model::load_material_texture(aiMaterial *mat, aiTextureType type, std::string type_name) {
    std::vector<Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type,i,&str);

        bool skip = false;
        for (size_t i = 0; i < loaded_textures.size() && !skip; i++) {
            if(loaded_textures[i].path.compare((dir + "/" + std::string(str.C_Str()))) == 0) {
                skip = true;
            }
        }

        if(!skip) {
            Texture texture((dir + "/" + std::string(str.C_Str())).c_str());
            loaded_textures.push_back(texture);
            textures.push_back(texture);
            // printf("loaded texture: %s\n",(dir + "/" + std::string(str.C_Str())).c_str());
        }
        
    }
    return textures;
}

void Model::draw(Shader& shader) {
    for (size_t i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}