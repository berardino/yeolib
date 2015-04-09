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
#include <yeo/math.hpp>
#include <yeo/io.hpp>

using namespace yeo;


std::string fileToString(std::string name) {
    std::ifstream t(name);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}


int main() {

    auto renderer = YeoCreateRenderer();

    auto device = renderer->CreateDevice();

    device->MakeCurrent();

    auto vShaderSource = fileToString("../../resources/SimpleVertexShader.vertexshader");
    auto vShader = device->CreateShader(vShaderSource, ShaderType::VERTEX);

    auto fShaderSource = fileToString("../../resources/SimpleFragmentShader.fragmentshader");
    auto fShader = device->CreateShader(fShaderSource, ShaderType::FRAGMENT);


    auto program = device->CreateProgram();

    program->Attach(vShader);
    program->Attach(fShader);
    program->Link();


    auto scene = YeoLoadScene("../../resources/models/1.5/cube.dae");

    static const float g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, };

    auto layout = device->CreateInputLayout()->Element()
    .Index(0)
    .Size(3)
    .Type(InputElementDesc::Type::FLOAT)
    .Normalized(false)
    .Stride(0)
    .Pointer((void*) 0).Add()
    .Build();


    auto vertex = device->CreateVertexBuffer();
    vertex->Write(sizeof(g_vertex_buffer_data), g_vertex_buffer_data, Buffer::STATIC_DRAW);

    while (!device->ShouldClose()) {

        program->Use();

        device->Clear();
        layout->Apply();

        vertex->Bind();
        
        device->Draw(DrawingMode::TRIANGLES, 0, 3);
        
        layout->UnApply();
        device->Present();
        
        renderer->PollEvents();
    }
    
    return 0;
}
