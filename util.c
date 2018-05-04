#include "header.h"
#include "util.h"
#include "svpng.inc"
#include <math.h>
#include <stdio.h>

extern double xpos, ypos;
extern double deltaX, deltaY;
extern double lastX, lastY;
extern int button_press_state;
extern double totalOffX, totalOffY;
extern double ratio;

// if get ESC
//     then exit
void key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // putchar(key);
    switch (key) {
        case 'Q':
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
            break;
        case 'S':
            printf("%s\n", "Saving Frame...");
            FILE* fp = fopen("out.png", "wb");
            unsigned char pixels[WIDTH * HEIGHT * 3];
            glReadPixels(0,0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            svpng(fp, WIDTH, HEIGHT, pixels, 0);
            fclose(fp);
            break;
        default:
            break;
    }
}

// if mouse press then
//     ones the press_state
//     update lastX and lastY
// else
//     zeros the press_state
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            button_press_state = 1;
            lastX = xpos;
            lastY = ypos;
        } else if (action == GLFW_RELEASE) {
            button_press_state = 0;
        }
    }
}

// if scroll then
//    update ratio by yoffset
// note: if yoffset == -1 the graph bllo up
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
    ratio = (pow(0.9, yoffset)) * ratio;
    // printf("r: %f\n", ratio);
}


// if press_state then
void cursor_position_state(void) {
    if (button_press_state) {
        totalOffX -= deltaX / ratio;
        totalOffY -= deltaY / ratio;
        deltaX = xpos - lastX;
        deltaY = ypos - lastY;
        totalOffX += deltaX / ratio;
        totalOffY += deltaY / ratio;
    } else {
        deltaX = 0.0;
        deltaY = 0.0;
    }
}
