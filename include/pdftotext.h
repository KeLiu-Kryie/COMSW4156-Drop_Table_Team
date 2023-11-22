#ifndef INCLUDE_PDFTOTEXT_H_
#define INCLUDE_PDFTOTEXT_H_

#include <glib.h>
#include <poppler-document.h>
#include <poppler-page.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

std::string data_to_text(const std::string& input_str);

// std::string file_to_text(const std::string& inputPdfPath);

#endif  // INCLUDE_PDFTOTEXT_H_
