#include "tesseract.h"

#include <iostream>

int main(int argc, char **argv)
{
        std::string path {argv[1]};
        std::string lang {argv[2]};
        std::cout << ocr::optical_character_recognition(path, lang) << "\n";
}
