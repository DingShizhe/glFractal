#define GLEW_STATIC
#include <GL/glew.h>

#include "ldsdr.h"
#include <stdio.h>
#include <stdlib.h>

// compile and load a fragment shader from a file

unsigned int v_shader = 0, f_shader = 0;

unsigned int create_shader(const char *shader_fname, unsigned int SHADER_TYPE) {

    // read from shader file
    FILE *shader_fp = fopen(shader_fname, "r");
    fseek(shader_fp, 0, SEEK_END);
    unsigned int shader_flen = ftell(shader_fp);
    fseek(shader_fp, 0, SEEK_SET);
    char *buffer = malloc(sizeof(char) * (shader_flen + 1));
    size_t sz = fread(buffer, 1, shader_flen, shader_fp);
    if (sz < 0) {
        fprintf(stderr, "%s\n", "Error: Read shader file failed.");
        exit(-1);
    }
    fclose(shader_fp);
    buffer[shader_flen] = '\0';
    // compile shader file
    unsigned int shader;
    shader = glCreateShader(SHADER_TYPE);
    glShaderSource(shader, 1, (const char **) &buffer, NULL);
    glCompileShader(shader);

    // handle exception
    int success;
    char infoLog[120];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    char *shader_type_str =
        (SHADER_TYPE == GL_VERTEX_SHADER) ? 
        "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER";
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "%s%s%s %s\n", 
            "ERROR::SHADER::",
            shader_type_str,
            "::COMPILATION_FAILED\n",
            infoLog);
    }

    free(buffer);
    return shader;
}


unsigned int create_prog(const char *v_shaderf, const char *f_shaderf) {

    unsigned int prog;
    prog = glCreateProgram();

    if (v_shaderf != NULL) {
        v_shader = create_shader(v_shaderf, GL_VERTEX_SHADER);
        glAttachShader(prog, v_shader);
    }

    if (f_shaderf != NULL) {
        f_shader = create_shader(f_shaderf, GL_FRAGMENT_SHADER);
        glAttachShader(prog, f_shader);
    }

    return prog;
}


void link_use_prog(unsigned int prog) {

    glLinkProgram(prog);

    int linked;
    char infoLog[120];
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        glGetProgramInfoLog(prog, 512, NULL, infoLog);
        fprintf(stderr, "%s %s\n", "ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog);
    }

    // glUseProgram(prog);
    return;
}


void delete_shaders(void) {
    if (v_shader != 0)
        glDeleteShader(v_shader);
    if (f_shader != 0)
        glDeleteShader(f_shader);
    return;
}

void set_uniform1f(unsigned int prog, const char *v_name, float val) {
    int loc = glGetUniformLocation(prog, v_name);
    if (loc != -1) {
        glUniform1f(loc, val);
    }
}