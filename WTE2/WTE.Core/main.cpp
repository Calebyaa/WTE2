#include <memory>

#include <wte/graphics/graphics.h>

#ifndef WTE_MAIN

int main() {

    auto renderer = std::make_shared<wte::graphics::Renderer>();
    
    renderer->Start();
    
    while (renderer->Render()) {}

    renderer->Shutdown();

    exit(EXIT_SUCCESS);
}

#endif // !WTE_MAIN