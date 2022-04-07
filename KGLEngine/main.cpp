// Developed by Kelin.Lyu.

#include "KGLEngine/core.hpp"

int main(int argc, char** argv) {
    
    Engine* engine = new Engine("KGLEngine", 0.5f, false, NULL);
    
    while(engine->isRunning()) {
        if(engine->shouldUpdate()) {
            
            engine->render();
        }
    }
    
    delete(engine);
    return(0);
}
