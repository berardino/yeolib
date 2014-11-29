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
using namespace std;
using namespace yeo;

int main() {
    
    std::shared_ptr<Renderer> renderer = YeoCreateRenderer();
    
    std::shared_ptr<Device> device = renderer->CreateDevice();
    
    device->MakeCurrent();
    
    std::cout << device->GetVersion() << "\n";
    
    
    while (!device->ShouldClose()) {
        device->Clear();
        device->Present();
        
        renderer->PollEvents();
        
        std::this_thread::sleep_for (std::chrono::seconds(1));
        
    }
    
    return 0;
}
