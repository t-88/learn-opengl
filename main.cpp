#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "pch.hpp"

#include "utils.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "model.hpp"



#define WIDTH  800
#define HEIGHT 600

static float dt;
float mouse_x;
float mouse_y;
bool show_mouse = false;

Camera camera;


void events(GLFWwindow* window);
void on_window_resize(GLFWwindow* window, int w, int h) {}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    mouse_x = xpos;
    mouse_y = ypos;
    if(show_mouse) return;
    camera.process_mouse_input(window,xpos,ypos);
}
GLFWwindow* glfw_opengl_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);


    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"42",NULL,NULL);
    if(window == NULL) {
        ERROR("failed to create window");
        glfwTerminate();
        exit(-1);
    }
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR("failed to load opengl function");
        glfwTerminate();
        exit(-1);
    }

    glViewport(0,0,WIDTH,HEIGHT);
    glfwSetFramebufferSizeCallback(window,on_window_resize);

    return window;
}
void imgui_init(GLFWwindow* window) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");
}


int main() {
    auto window =  glfw_opengl_init();
    imgui_init(window);
    stbi_set_flip_vertically_on_load(true);

    camera = Camera(WIDTH,HEIGHT);

    glm::mat4 view = glm::mat4(1.);
    glm::mat4 proj = glm::perspective(glm::radians(45.f),(float)WIDTH/HEIGHT,0.1f,100.f);
    glm::mat4 model = glm::translate(glm::mat4(1.f),glm::vec3(0,0,-0.5));

    Shader shader_prog_obj("shader.vert","shader.frag");


    Shader shader_prog_light("shader.vert","shader_light.frag");
    glm::mat4 model_light = glm::mat4(1.);
    glm::vec3 light_color = {1.,1.,1.};
    glm::vec3 light_pos = {-1.,0.6,1.};
    model_light = glm::scale(glm::translate(model_light,light_pos),glm::vec3(0.5));
    shader_prog_light.enable();
    shader_prog_light.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog_light.set_vec3("light_color",{light_color[0],light_color[1],light_color[2]});


    shader_prog_obj.enable();
    shader_prog_obj.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog_obj.set_mat4x4("model",glm::value_ptr(model));
    shader_prog_obj.set_vec3("light.position",{light_pos[0],light_pos[1],light_pos[2]});
    
    
    camera.pos = glm::vec3(0.,0.,4.);
    camera.front = glm::vec3(0.,0.,0.);
    camera.up = glm::vec3(0.,1.,0.);
  
    Model model_cube("assets/cube/cube.obj");
    Model model_backpack("assets/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);
    float t , last_update;
    while (!glfwWindowShouldClose(window)) {
        t = glfwGetTime();
        dt = t - last_update;
        last_update = t;

        events(window);  
        
        view = camera.get_view_matrix();

        shader_prog_light.enable();
        shader_prog_light.set_mat4x4("view",glm::value_ptr(view));
        shader_prog_light.set_mat4x4("model",glm::value_ptr(model_light));
        model_cube.draw(shader_prog_light);


        shader_prog_obj.enable();
        shader_prog_obj.set_mat4x4("view",glm::value_ptr(view));
        model_backpack.draw(shader_prog_obj);


        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if(ImGui::ColorPicker3("light color",&light_color[0])) {
                shader_prog_light.enable();
                shader_prog_light.set_vec3("light_color",{light_color[0],light_color[1],light_color[2]});
            }

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
        glClearColor(0.f,0.f,0.f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    shader_prog_light.free();

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void events(GLFWwindow* window) {
    static bool space_pressed = false;

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
            camera.mouse_last_x = mouse_x;
            camera.mouse_last_y = mouse_y;
        }
    } else if (glfwGetKey(window,GLFW_KEY_SPACE) != GLFW_PRESS) {
        space_pressed = false;
    }
    camera.process_keyboard_input(window,dt);
}

