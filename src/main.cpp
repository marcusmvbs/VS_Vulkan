#include "app.hpp"

#include "/opt/homebrew/include/GLFW/glfw3.h"

#include <iostream> //Reporting and propagating errors
#include <stdexcept> //Reporting and propagating errors
#include <cstdlib> // EXIT_SUCCESS and EXIT_FAILURE macros

int main() {

    lve::First_app app{};
    
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}