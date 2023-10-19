#ifndef OCR_H
#define OCR_H

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <iostream>
#include <vector>
#include <string>

namespace ocr
{

std::string optical_character_recognition(const std::string& path, const std::string& language);

std::string image_to_text(const std::string& data, const std::string& language);

}

#endif
