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
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };


    uint32_t vao;
    glGenVertexArrays(1,&vao);
    uint32_t vbo; 
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 3,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    Shader shader_prog_obj("examples/colors_shader.vert","examples/colors_shader.frag");
    Shader shader_prog_light("examples/colors_shader.vert","examples/colors_shader_light.frag");


    camera = (camera_t){

        glm::vec3(0.,0.,4.),
        glm::vec3(0.,0.,0.),
        glm::vec3(0.,1.,0.),
    };


    glm::mat4 model = glm::mat4(1.);
    glm::mat4 view = glm::mat4(1.);
    glm::mat4 proj = glm::perspective(glm::radians(45.f),(float)WIDTH/HEIGHT,0.1f,100.f);

    shader_prog_obj.enable();
    shader_prog_obj.set_mat4x4("view",glm::value_ptr(view));
    shader_prog_obj.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog_obj.set_mat4x4("model",glm::value_ptr(model));

    shader_prog_light.enable();
    shader_prog_light.set_mat4x4("view",glm::value_ptr(view));
    shader_prog_light.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog_light.set_mat4x4("model",glm::value_ptr(model));

    

    glm::mat4 model_obj = glm::mat4(1.);
    model_obj = glm::translate(model_obj,glm::vec3(1,0.,-0.5));

    glm::mat4 model_light = glm::mat4(1.);
    model_light = glm::translate(model_light,glm::vec3(-1.,0.,0.5));
    model_light = glm::scale(model_light,glm::vec3(0.5));


    std::vector<float> light_color = {1.,0.25,1.};
    shader_prog_light.enable();
    shader_prog_light.set_vec3("light_color",light_color);

    std::vector<float> obj_color = {0.25,0.3,1.};
    shader_prog_obj.enable();
    shader_prog_obj.set_vec3("obj_color",obj_color);
    shader_prog_obj.set_vec3("light_color",light_color);



    float t;
    float last_time;

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  
    while (!glfwWindowShouldClose(window)) {
        // dt
        t = glfwGetTime();
        dt = t - last_time;
        last_time = t;

        // input
        glfwPollEvents();
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window,true);
        }
        camera.process_input(window);


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

        
        


     
        


        shader_prog_light.enable();
        shader_prog_light.set_mat4x4("view",glm::value_ptr(view));
        shader_prog_light.set_mat4x4("model",glm::value_ptr(model_light));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        shader_prog_obj.enable();
        shader_prog_obj.set_mat4x4("view",glm::value_ptr(view));
        shader_prog_obj.set_mat4x4("model",glm::value_ptr(model_obj));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        glfwSwapBuffers(window);
        glClearColor(0.f,0.f,0.f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    shader_prog_obj.free();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
