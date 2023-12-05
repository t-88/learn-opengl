#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"
#include "shader.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIDTH  800
#define HEIGHT 600

static float dt;
float mouse_last_x = 400, mouse_last_y = 300;
float pitch  =  0.0, yaw = -90.0;
bool init_mouse = false;



typedef struct camera_t {
    glm::vec3 pos, front , up; 
    float speed = 5.f;

    void process_input(GLFWwindow* window) {
        if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
            pos += speed * dt * front;
        } else if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS) {
            pos -= speed * dt * front;
        }

        if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) {
            pos += speed * dt * glm::normalize(glm::cross(front,up));
        } else if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) {
            pos -= speed * dt * glm::normalize(glm::cross(front,up));
        }
    } 
} camera_t;
static camera_t camera;

void on_window_resize(GLFWwindow* window, int w, int h) {
    // do nothing window is not resizable
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(!init_mouse) {
        mouse_last_x = xpos;
        mouse_last_y = ypos;
        init_mouse = true;
        return;
    }

    float xoffset = xpos - mouse_last_x;
    float yoffset = - ypos + mouse_last_y;

    mouse_last_x = xpos; 
    mouse_last_y = ypos;

    xoffset *= 0.1;
    yoffset *= 0.1;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }
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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 5,0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float) * 5,(void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    Shader shader_prog("shader.vert","shader.frag");


    int tex_w,tex_h;
    stbi_set_flip_vertically_on_load(true);
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


    camera = (camera_t){
        glm::vec3(0.,0.,5.),
        glm::vec3(0.,0.,-1.),
        glm::vec3(0.,1.,0.),
    };


    glm::mat4 model = glm::mat4(1.);
    glm::mat4 view = glm::mat4(1.);
    glm::mat4 proj = glm::perspective(glm::radians(45.f),(float)WIDTH/HEIGHT,0.1f,100.f);

    shader_prog.enable();

    shader_prog.set_mat4x4("view",glm::value_ptr(view));
    shader_prog.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog.set_mat4x4("model",glm::value_ptr(model));

    

    glm::vec3 center_cube_pos   = glm::vec3(0.f,0.f,-2.f);
    float center_cube_rotation   = 0.f;

    glm::vec3 rotating_cube_pos   = glm::vec3(0.f,0.f,-4.f);
    float rotating_cube_rotation   = 0.f;



  


    float t;
    float last_time;

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  
    while (!glfwWindowShouldClose(window)) {
        
        t = glfwGetTime();
        dt = t - last_time;
        last_time = t;



        glfwPollEvents();

        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window,true);
        }


        camera.process_input(window);



        shader_prog.enable();

        view = glm::lookAt(
            camera.pos,
            camera.pos + camera.front,
            camera.up
        );

        glm::vec3 dir;
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch)) ;
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera.front = glm::normalize(dir);

        shader_prog.set_mat4x4("view",glm::value_ptr(view));
        
        
	
	    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex_id);

        glBindVertexArray(vao);
        

        center_cube_rotation += 2;
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model,center_cube_pos);
        model = glm::rotate(model,glm::radians(center_cube_rotation),glm::vec3(1.f,1.f,1.f));
        shader_prog.set_mat4x4("model",glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        rotating_cube_rotation += 1; 
        model = glm::mat4(1.f);
        model = glm::translate(model,rotating_cube_pos);
        model = glm::rotate(model,glm::radians(rotating_cube_rotation),glm::vec3(1.f,0.f,0.f));
        model = glm::rotate(model,glm::radians(rotating_cube_rotation / 2),glm::vec3(0.f,1.f,0.f));
        model = glm::rotate(model,glm::radians(rotating_cube_rotation / 4),glm::vec3(0.f,0.f,1.f));
        model = glm::translate(model,glm::vec3(0.f,0.,3.));
        shader_prog.set_mat4x4("model",glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);





        glfwSwapBuffers(window);
        glClearColor(0.f,0.f,0.f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    shader_prog.free();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
