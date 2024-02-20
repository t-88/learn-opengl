#include "camera.hpp"




Camera::Camera() {}
Camera::Camera(int w, int h) {
    mouse_last_x = w / 2, mouse_last_y = h / 2;
}
void Camera::process_keyboard_input(GLFWwindow* window,float dt) {
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
void Camera::process_mouse_input(GLFWwindow* window, double xpos, double ypos) {
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

glm::mat4 Camera::get_view_matrix() {
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