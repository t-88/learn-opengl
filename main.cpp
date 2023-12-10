#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "pch.hpp"

#include "utils.hpp"
#include "shader.hpp"
#include "texture.hpp"




#define WIDTH  800
#define HEIGHT 600

static float dt;
float mouse_x;
float mouse_y;
bool show_mouse = false;

typedef struct camera_t {
    glm::vec3 pos, front , up; 
    float pitch  =  0.0,
          yaw = -90.0;
    float mouse_last_x = WIDTH / 2,
          mouse_last_y = HEIGHT / 2;
    float speed = 5.f;
    bool init_mouse = false;


    void process_keyboard_input(GLFWwindow* window) {
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
    void process_mouse_input(GLFWwindow* window, double xpos, double ypos) {

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
        glm::mat4 get_view_matrix() {
            glm::vec3 dir;
            dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            dir.y = sin(glm::radians(pitch)) ;
            dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(dir);
            return glm::lookAt(
                pos,
                pos + front,
                up
            );
        }

} camera_t;
static camera_t camera;



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


void events(GLFWwindow* window);




int main() {
    auto window = 
    
    glfw_opengl_init();
    imgui_init(window);


    float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    uint32_t vao;
    uint32_t vbo; 

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(float*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    Shader shader_prog_obj("shader.vert","shader.frag");
    Shader shader_prog_light("shader.vert","shader_light.frag");



    glm::mat4 view = glm::mat4(1.);
    glm::mat4 proj = glm::perspective(glm::radians(45.f),(float)WIDTH/HEIGHT,0.1f,100.f);

    glm::mat4 model_obj = glm::mat4(1.);
    model_obj = glm::translate(model_obj,glm::vec3(1,-0.2,0.5));
    std::vector<float> obj_color = {1.,1.,1.};

    glm::mat4 model_light = glm::mat4(1.);
    std::vector<float> light_color = {1.,1.,1.};
    std::vector<float> light_pos = {-1.,0.6,1.};
    model_light = glm::translate(model_light,glm::vec3(light_pos[0],light_pos[1],light_pos[2]));
    model_light = glm::scale(model_light,glm::vec3(0.5));

    shader_prog_obj.enable();
    shader_prog_obj.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog_obj.set_vec3("light.specular",{1.,1.,1.});
    shader_prog_obj.set_vec3("light.diffuse",{0.7,0.7,0.7});
    shader_prog_obj.set_vec3("light.ambient",{0.2,0.2,0.2});
    shader_prog_obj.set_vec3("light.position",light_pos);
    shader_prog_obj.set_float("light.constant",  1.0f);
    shader_prog_obj.set_float("light.linear",    0.09f);
    shader_prog_obj.set_float("light.quadratic", 0.032f);	
    
    shader_prog_obj.set_int("material.diffuse",0);
    shader_prog_obj.set_int("material.specular",1);


    

    int shininess = 32;
    shader_prog_obj.set_float("material.shininess",shininess);


    shader_prog_light.enable();
    shader_prog_light.set_mat4x4("proj",glm::value_ptr(proj));
    shader_prog_light.set_vec3("light_color",light_color);


    
    Texture texture("assets/container2.png");
    Texture texture_specular("assets/container2_specular.png");

    float t;
    float last_update;
    camera = (camera_t){
        glm::vec3(0.,0.,4.),
        glm::vec3(0.,0.,0.),
        glm::vec3(0.,1.,0.),
    };

    std::vector<glm::vec3> poses = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

  
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        t = glfwGetTime();
        dt = t - last_update;
        last_update = t;


        events(window);  
        
        view = camera.get_view_matrix();
        

        shader_prog_light.enable();
        shader_prog_light.set_mat4x4("view",glm::value_ptr(view));
        shader_prog_light.set_mat4x4("model",glm::value_ptr(model_light));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        texture.bind();
        texture_specular.bind(1);


        shader_prog_obj.enable();
        shader_prog_obj.set_mat4x4("view",glm::value_ptr(view));
        shader_prog_obj.set_vec3("light.position",{camera.pos.x,camera.pos.y,camera.pos.z});
        shader_prog_obj.set_vec3("light.direction", {camera.front.x,camera.front.y,camera.front.z});
        shader_prog_obj.set_float("light.cutOff",   glm::cos(glm::radians(12.5)));
        shader_prog_obj.set_float("light.outer_cutOff",   glm::cos(glm::radians(20.)));
        glBindVertexArray(vao);

        float angle = 0;
        for (size_t i = 0; i < poses.size(); i++) {
            glm::mat4 model_obj = glm::mat4(1.);
            model_obj = glm::translate(model_obj,poses[i]);
            model_obj = glm::rotate(model_obj,glm::radians(angle),glm::vec3(1.,1.,0.));

            angle += 25;

            shader_prog_obj.set_mat4x4("model",glm::value_ptr(model_obj));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        



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
        
        ImGui::SliderInt("shininess",&shininess,0,256);
        shader_prog_obj.enable();
        shader_prog_obj.set_float("material.shininess",shininess);

        ImGui::EndFrame();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
     


        glfwSwapBuffers(window);
        glClearColor(0.f,0.f,0.f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    shader_prog_obj.free();
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
    camera.process_keyboard_input(window);
}

