#include <wte/graphics/graphics.h>
#include <memory>

int main() {
    auto renderer = std::make_shared<wte::graphics::Renderer>();
    renderer->Start();
    while(renderer->Render()) {}
    renderer->Shutdown();
}