#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>

#include "pch.hpp"

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
bool show_mouse = false;

double mouse_x;
double mouse_y;


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
    if(show_mouse) return;

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
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  



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


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");


    float vertices[] = {
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };


    uint32_t vao;
    glGenVertexArrays(1,&vao);
    uint32_t vbo; 
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 6,(float*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float) * 6,(void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    Shader shader_prog_obj("shader.vert","shader.frag");
    Shader shader_prog_light("shader.vert","shader_light.frag");


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
    model_obj = glm::scale(model_obj,glm::vec3(2,1.,1.));

    glm::mat4 model_light = glm::mat4(1.);
    std::vector<float> light_pos = {-1.,0.,0.5};
    model_light = glm::translate(model_light,glm::vec3(light_pos[0],light_pos[1],light_pos[2]));
    model_light = glm::scale(model_light,glm::vec3(0.5));


    std::vector<float> light_color = {1.,0.25,1.};
    shader_prog_light.enable();
    shader_prog_light.set_vec3("light_color",light_color);

    std::vector<float> obj_color = {0.25,0.3,1.};
    shader_prog_obj.enable();
    shader_prog_obj.set_vec3("obj_color",obj_color);
    shader_prog_obj.set_vec3("light_color",light_color);
    shader_prog_obj.set_vec3("light_pos",light_pos);



    float t;
    float last_time;

    bool space_pressed = false;
            static int counter = 0;

    glEnable(GL_DEPTH_TEST);
    

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
        if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS && !space_pressed) {
            space_pressed = true;
            show_mouse = !show_mouse;
            if(show_mouse) {
                glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
            }
        } else if (glfwGetKey(window,GLFW_KEY_SPACE) != GLFW_PRESS) {
            space_pressed = false;
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
        shader_prog_obj.set_vec3("camera_pos",{camera.pos.x,camera.pos.y,camera.pos.z});

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);





        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::ColorPicker3("light color",&light_color[0])) {
            shader_prog_light.enable();
            shader_prog_light.set_vec3("light_color",light_color);

            std::vector<float> obj_color = {0.25,0.3,1.};
            shader_prog_obj.enable();
            shader_prog_obj.set_vec3("obj_color",obj_color);
            shader_prog_obj.set_vec3("light_color",light_color);
        }
        
        if(ImGui::Button("reset position")) {
            pitch = 0.;
            yaw = -90.;
            camera = (camera_t){
                glm::vec3(0.,0.,4.),
                glm::vec3(0.,0.,0.),
                glm::vec3(0.,1.,0.),
            };
        }
        ImGui::EndFrame();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
     


        glfwSwapBuffers(window);
        glClearColor(0.f,0.f,0.f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    shader_prog_obj.free();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
