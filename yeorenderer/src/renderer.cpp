/**
 This file is part of YeoLibrary.
 
 YeoLibrary is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 3.
 
 YeoLibrary is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with YeoLibrary.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <yeo/renderer.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#define YEO_DLL_EXPORTS

using namespace yeo;
using namespace std;


class OpenGLDevice : public Device {
private:
    GLFWwindow* window;
    
public:
    OpenGLDevice(GLFWwindow* window) {
        this->window = window;
    }
    
    ~OpenGLDevice() {
        glfwDestroyWindow(window);
    }
    
    void MakeCurrent() {
        glfwMakeContextCurrent(window);
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        }
        glEnable(GL_SCISSOR_TEST);
    }
    
    void Present() {
        glfwSwapBuffers(window);
    }
    
    int ShouldClose() {
        return glfwWindowShouldClose(window);
    }
    
    void SetViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
        glScissor(x, y, width, height);
    }

    void GetFramebufferSize(int* width, int* height) {
        glfwGetFramebufferSize(window, width, height);
    }
    
    void Clear() {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SetProjectionMatrix(const float4x4& matrix) {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(matrix.memptr());
    }

    void SetViewMatrix(const float4x4& matrix) {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(matrix.memptr());
    }

    string GetVersion() {
        return reinterpret_cast<char const *>(glGetString(GL_VERSION));
    }
};

class OpenGLRenderer : public Renderer {
public:
    ~OpenGLRenderer() {
        glfwTerminate();
    }
    
    shared_ptr<Device> CreateDevice() {
        GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        return shared_ptr<Device>(new OpenGLDevice(window));
    }
    
    void PollEvents(){
        glfwPollEvents();
    }

    void WaitEvents(){
        glfwWaitEvents();
    }

};

YEO_API shared_ptr<Renderer> YeoCreateRenderer(void){
    if (!glfwInit()) {
        return shared_ptr<Renderer>();
    }
    return shared_ptr<Renderer>(new OpenGLRenderer());
}

