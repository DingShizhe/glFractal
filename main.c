#define GLEW_STATIC
#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ldsdr.h"
#include "util.h"
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

// cursor position at each momment
double xpos, ypos;

// position defferent of xpos and last position when pressed
double deltaX, deltaY;

// cursor position at the momment of last time press
double lastX = (WIDTH  / 2.0);
double lastY = (HEIGHT / 2.0);

// value pass to shader
double totalOffX = 0.0, totalOffY = 0.0;
double ratio = 1.0;

int button_press_state = 0;
double timeStamp = 0.0;
double deltaTime = 0.0;
int count = 0;

unsigned int prog;

int main(int argc, char *argv[]) {
    unsigned char arg;
    if (argc != 2) {
        fprintf(stderr, "%s\n", "Usage: ./fractal <option>");
        arg = '1';
        // exit(0);
    } else {
        arg = argv[1][0];
    }

    char *v_shaderf, *f_shaderf;
    v_shaderf = "shaders/static.vert";
    f_shaderf = "shaders/static.frag";
    switch (arg) {
        case '0' : f_shaderf = "shaders/mandelbulb.frag"; break;
        case '1' : f_shaderf = "shaders/mandelbrot1.frag"; break;
        case '2' : f_shaderf = "shaders/mandelbrot2.frag"; break;
        case '3' : f_shaderf = "shaders/mandelbrot3.frag"; break;
        case '4' : f_shaderf = "shaders/test.frag"; break;
        case '5' : f_shaderf = "shaders/static.frag"; break;
        case '6' : f_shaderf = "shaders/julia.frag"; break;
        default: fprintf(stderr, "%s\n", "Usage: ./fractal <option>"); exit(0);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_press_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glViewport(0, 0, WIDTH, HEIGHT);

    // OpenGL Context Init
    glewExperimental = GL_TRUE;
    glewInit();

    prog = create_prog(v_shaderf, f_shaderf);
    link_use_prog(prog);
    delete_shaders();

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    timeStamp = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        set_uniform1f(prog, "totalOffX", totalOffX);
        set_uniform1f(prog, "totalOffY", -totalOffY);
        set_uniform1f(prog, "ratio", ratio);

        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwGetCursorPos(window, &xpos, &ypos);
        cursor_position_state();
        glfwPollEvents();

        count ++;
        if (count % 100 == 0) {
            count %= 100;
            deltaTime = glfwGetTime() - timeStamp;
            timeStamp = glfwGetTime();
            printf("%f FPS\n", 100 / deltaTime);
        }
        fflush(stdout);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
