#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Camera {
public:
    glm::vec3 pos, front , up; 
    float pitch  =  0.0, yaw = -90.0;
    float mouse_last_x = 500, mouse_last_y = 500;
    float speed = 5.f;
    bool init_mouse = false;


    Camera();
    Camera(int w, int h);
    void process_keyboard_input(GLFWwindow* window,float dt);
    void process_mouse_input(GLFWwindow* window, double xpos, double ypos);
    glm::mat4 get_view_matrix();
};