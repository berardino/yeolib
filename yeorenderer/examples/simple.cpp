//============================================================================
// Name        : glcpp.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include <yeo/renderer.hpp>
#include <yeo/math.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;
using namespace yeo;

float i = 1.f;


// Define the 8 vertices of a unit cube
float g_Vertices[48] = {
      1.f,  1.f,  1.f, 1.f, 1.f, 1.f , // 0
     -1.f,  1.f,  1.f, 0.f, 1.f, 1.f , // 1
     -1.f, -1.f,  1.f, 0.f, 0.f, 1.f , // 2
      1.f, -1.f,  1.f, 1.f, 0.f, 1.f , // 3
      1.f, -1.f, -1.f, 1.f, 0.f, 0.f , // 4
     -1.f, -1.f, -1.f, 0.f, 0.f, 0.f , // 5
     -1.f,  1.f, -1.f, 0.f, 1.f, 0.f , // 6
      1.f,  1.f, -1.f, 1.f, 1.f, 0.f , // 7
};

GLuint g_Indices[24] = {
    0, 1, 2, 3,                 // Front face
    7, 4, 5, 6,                 // Back face
    6, 5, 2, 1,                 // Left face
    7, 0, 3, 4,                 // Right face
    7, 6, 1, 0,                 // Top face
    3, 2, 5, 4,                 // Bottom face
};

GLuint g_uiVerticesVBO = 0;
GLuint g_uiIndicesVBO = 0;

void draw(std::shared_ptr<Device> device) {
    float ratio;
    int width, height;
    device->GetFramebufferSize(&width, &height);
    ratio = (float)width / (float) height;
    device->SetViewport(0, 0, width, height);
    
    float4x4 projection;
    YeoOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f, projection);
    device->SetProjectionMatrix(projection);
    
    float4x4 view;
    i=i+0.01;
    float angle = (float) (i) * 1.f;
    float3 axis;
    axis << 0.f << 0.f << 1.f;
    YeoRotate(angle, axis, view);
    device->SetViewMatrix(view);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_uiVerticesVBO );
    glVertexPointer( 3, GL_FLOAT, sizeof(float), (void*)0 );
    glColorPointer( 3, GL_FLOAT, sizeof(float), (void*)(sizeof(float)*3) );
    
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, g_uiIndicesVBO );

    glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT,  0);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

}

int main() {
    
    std::shared_ptr<Renderer> renderer = YeoCreateRenderer();
    
    std::shared_ptr<Device> device = renderer->CreateDevice();
    
    device->MakeCurrent();
    
    std::cout << device->GetVersion() << "\n";
    
    glGenBuffersARB( 1, &g_uiVerticesVBO );
    glGenBuffersARB( 1, &g_uiIndicesVBO );
    
    // Copy the vertex data to the VBO
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_uiVerticesVBO );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(g_Vertices), g_Vertices, GL_STATIC_DRAW_ARB );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    
    // Copy the index data to the VBO
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, g_uiIndicesVBO );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(g_Indices), g_Indices, GL_STATIC_DRAW_ARB );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
    
    
    while (!device->ShouldClose()) {
        
        device->Clear();
        
        draw(device);
        
        device->Present();
        
        renderer->PollEvents();
    }
    
    return 0;
}
