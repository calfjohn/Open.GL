//
//  drawing.cpp
//  OpenGL VAO,VBO,EBO示例
//
//  Created by chenbingfeng on 15/4/25.
//  Copyright (c) 2015年 chenbingfeng. All rights reserved.
//

#include "drawing.h"
#include <math.h>
#include <unistd.h>

#include <iostream>
#include <chrono>

#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>

#include "utils.h"

static GLfloat vertices[] = {
    0.0f, 0.5f, 1.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.3f,
    1.0f, 0.5f, 0.8f,
};

static GLuint elements[] = {
    0,1,2,
    0,1,3
};

static const char* vertexSource =
"#version 150\n"
"\n"
"in vec2 position;\n"
"in float color;\n"
"out vec3 Color;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    Color = vec3(color);\n"
"    gl_Position = vec4(position.x, -position.y, 0.0, 1.0);\n"
"}\n";

static const char* fragmentSource =
"#version 150\n"
"\n"
"in vec3 Color;\n"
"uniform vec3 triangleColor;\n"
"\n"
"out vec4 outColor;\n"
"\n"
"void main()\n"
"{\n"
"    outColor = vec4(vec3(1.0f,1.0f,1.0f) - Color, 1.0);\n"
"}\n";
static char clog[2014];
static GLuint vbo;
static GLuint vao;
static GLuint ebo;
static GLint uniColor;
static GLuint textureId;
static decltype(std::chrono::high_resolution_clock::now()) t_start;
static void init()
{
    LOG("GL_RENDERER", glGetString(GL_RENDERER),  " GL_VERSION = ",glGetString(GL_VERSION));
    
    //vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    //vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    //shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        LOG("vertex shader compile failed.");
        glGetShaderInfoLog(vertexShader, 1024, NULL, clog);
        LOG(clog);
        exit(1);
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        LOG("fragment shader compile failed.");
        glGetShaderInfoLog(fragmentShader, 1024, NULL, clog);
        LOG(clog);
        exit(1);
    }
    
    //as program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    //attributes
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)(2*sizeof(float)));
    
    uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
    
    
    //    glGenTextures(1, &textureId);
    //    glBindTexture(GL_TEXTURE_2D, textureId);
    t_start = std::chrono::high_resolution_clock::now();
    
}

static void display()
{
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
    glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glutSwapBuffers();
    sleep(0.33);
    glutPostRedisplay();
}

static void reshape(int w, int h)
{
    
}

void drawing(){
    LOG("gl Test Code");
    int argc = 0;
    char * argv[] = {0};
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_3_2_CORE_PROFILE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("lol model view");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
}