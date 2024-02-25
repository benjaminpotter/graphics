
#include <iostream>
#include "GraphicsApplication.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080


int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;

    GraphicsApplication app("My First Window", WINDOW_WIDTH, WINDOW_HEIGHT);

    // initialize app

    app.start();

    return 0;
}