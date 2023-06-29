//
// Created by William.Hua on 2020/9/2.
//
#include "mp3ToPcm.cpp"
#include "play.cpp"
//-------

#include <iostream>
#include <map>
#include <string>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <learnopengl/shader_s.h>
#include <learnopengl/filesystem.h>
#include <iostream>

class yuvFileChunk {
public:
    yuvFileChunk(int _pixel_w = 320, int _pixel_h = 180) {
        pixel_w = _pixel_w;
        pixel_h = _pixel_h;
        yuvBuffer = new unsigned char[pixel_w * pixel_h * 3 / 2];
    }

    int readYuv(const char *path) {
        if ((infile = fopen(path, "rb")) == NULL) {
            printf("cannot open this file\n");
            return -1;
        }
        //YUV Data
        plane[0] = yuvBuffer;
        plane[1] = plane[0] + pixel_w * pixel_h;
        plane[2] = plane[1] + pixel_w * pixel_h / 4;
    };

    void display() const {
        if (fread(yuvBuffer, 1, pixel_w * pixel_h * 3 / 2, infile) != pixel_w * pixel_h * 3 / 2) {
            // Loop
            fseek(infile, 0, SEEK_SET);
            fread(yuvBuffer, 1, pixel_w * pixel_h * 3 / 2, infile);
        }
    }

    FILE *infile;
    int pixel_w;
    int pixel_h;
    unsigned char *yuvBuffer;
    unsigned char *plane[3];
};

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;

void go(int argc, char *argv[]) {
    // FFmpeg 解码
//    mp3ToPcm(
//            "/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.mp3",
//            "/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.pcm"
//            );

    // openal 播放裸流
    play("/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.mp3");
}
int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("/Users/panfeng/coder/learn/opengl/openglStudy/shader_vs.vs",
                     "/Users/panfeng/coder/learn/opengl/openglStudy/shader_fs.fs");

    ourShader.use();
    GLint textureUniformY = glGetUniformLocation(ourShader.ID, "tex_y");
    GLint textureUniformU = glGetUniformLocation(ourShader.ID, "tex_u");
    GLint textureUniformV = glGetUniformLocation(ourShader.ID, "tex_v");

    auto yuvFileObj = new yuvFileChunk();
    // yuvFileObj->readYuv("/Users/panfeng/coder/learn/opengl/openglStudy/container_qcif.yuv");

    yuvFileObj->readYuv("/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/media/video.yuv");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // init
    // -------------------------
    //Y
    unsigned int id_y;
    glGenTextures(1, &id_y);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);

    unsigned int id_u;
    glGenTextures(1, &id_u);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);

    unsigned int id_v;
    glGenTextures(1, &id_v);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);



    // render loop
    // -----------
    int time = 0;

    play("/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.pcm");

    while (!glfwWindowShouldClose(window)) {

        // render
        // ------
        // yuvFileObj->display();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // render container
        ourShader.use();
        if (time++ > 20) {
            time = 0;
            yuvFileObj->display();
            // init
            // -------------------------

            // std::cout << yuvFileObj->plane[0] << std::endl;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id_y);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, yuvFileObj->pixel_w, yuvFileObj->pixel_h, 0, GL_RED,
                         GL_UNSIGNED_BYTE,
                         yuvFileObj->plane[0]);
            glUniform1i(textureUniformY, 0);

            //U
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, id_u);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, yuvFileObj->pixel_w / 2, yuvFileObj->pixel_h / 2, 0, GL_RED,
                         GL_UNSIGNED_BYTE,
                         yuvFileObj->plane[1]);
            glUniform1i(textureUniformU, 1);
            //V
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, id_v);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, yuvFileObj->pixel_w / 2, yuvFileObj->pixel_h / 2, 0, GL_RED,
                         GL_UNSIGNED_BYTE, yuvFileObj->plane[2]);
            glUniform1i(textureUniformV, 2);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // glfwTerminate();
    return 0;
}



