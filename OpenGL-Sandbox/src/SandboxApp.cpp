#include "GLCore.h"
#include "SandboxLayer.h"

using namespace GLCore;

class Sandbox : public Application
{
public:
    Sandbox()
    {
        PushLayer(new SandboxLayer());
    }
};

// Force rendering on NVIDIA GPU
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// Force rendering on AMD GPU
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main()
{
    std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
    app->Run();
}