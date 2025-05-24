#include "Lexer.h"
#include "RecursiveDescendant.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <python_file>" << std::endl;
        return 1;
    }
    
    try {
        Lexer lexer(argv[1]);
        RecursiveDescendant parser(&lexer);
        parser.parse();
    } catch (int num) {
    // } catch (const std::exception& e) {
        // std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}