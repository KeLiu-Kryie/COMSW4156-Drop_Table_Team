#ifndef PDFTOTEXT_H
#define PDFTOTEXT_H

#include <poppler-document.h>
#include <poppler-page.h>
#include <iostream>
#include <fstream>
#include <glib.h>
#include <algorithm>

std::string data_to_text(const std::string& input_str);

std::string file_to_text(const std::string& inputPdfPath);

#endif
