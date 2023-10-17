#include <poppler-document.h>
#include <poppler-page.h>
#include <iostream>
#include <fstream>
#include <glib.h>

std::string data_to_text(const std::string& input_str)
{

        using namespace poppler;

        byte_array data(input_str.begin(), input_str.end());

        document *doc = document::load_from_data(&data);

        if (!doc)
                return "UNABLE TO READ PDF";

        int numPages = doc->pages();
        std::cerr << numPages << "\n";
        std::string output = "";

        for (int i = 0; i < numPages; ++i) {
                const page *pdfPage = doc->create_page(i);
                auto text = pdfPage->text().to_latin1();
                output += text;
        }

        std::cerr << output << "\n";
        return output;

}

std::string file_to_text(const std::string& inputPdfPath)
{
        using namespace poppler;

        document *doc = document::load_from_file(inputPdfPath.c_str());

        if (!doc)
                return "UNABLE TO READ PDF";

        int numPages = doc->pages();
        std::string output = "";

        for (int i = 0; i < numPages; ++i) {
                const page *pdfPage = doc->create_page(i);
                auto text = pdfPage->text().to_latin1();
                output += text;
        }

        return output;
}
