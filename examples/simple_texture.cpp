#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "shader.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIDTH  800
#define HEIGHT 600



void on_window_resize(GLFWwindow* window, int w, int h) {
    // do nothing window is not resizable
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);


    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"42",NULL,NULL);
    
    if(window == NULL) {
        ERROR("failed to create window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR("failed tp load opengl function");
        glfwTerminate();
        return -1;
    }

    glViewport(0,0,WIDTH,HEIGHT);
    glfwSetFramebufferSizeCallback(window,on_window_resize);



    float vertices[] = {
        -0.5,  -0.5,  0., 1. ,0. ,0., 0.0, 0.0,
        -0.5,   0.5,  0., 0. ,1. ,0., 0.0, 1.0,
         0.5,  -0.5,  0., 0. ,0. ,1., 1.0, 0.0,
         0.5,   0.5,  0., 1. ,1. ,0., 1.0, 1.0,
    };

    uint32_t indices[] = {
        0 , 1 , 2,
        1 , 2 , 3,
    };


    uint32_t vao;
    glGenVertexArrays(1,&vao);
    uint32_t vbo; 
    glGenBuffers(1,&vbo);
    uint32_t ebo;
    glGenBuffers(1,&ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 8,0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(void*) (sizeof(float) * 6));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    Shader shader_prog("shader.vert","shader.frag");


    int tex_w,tex_h;
    uint8_t* tex_data = stbi_load("./assets/container.jpg",&tex_w,&tex_h,0,3);
    if(!tex_data) {
        ERROR("failed to load img");
        glfwTerminate();
        exit(-1);
    }

    uint32_t tex_id;
    glGenTextures(1,&tex_id);
    glBindTexture(GL_TEXTURE_2D,tex_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tex_w,tex_h,0,GL_RGB,GL_UNSIGNED_BYTE,tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_data);



    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window,true);
        }

        shader_prog.enable();
	
	glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex_id);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,indices);


        glfwSwapBuffers(window);
        glClearColor(0.f,0.f,0.f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    shader_prog.free();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
