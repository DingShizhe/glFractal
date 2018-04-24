#ifndef _UTIL_
#define _UTIL_

#include <GLFW/glfw3.h>

void key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_state(void);

#endif