#include "../include/pdftotext.h"

std::string data_to_text(const std::string& input_str)
{
        using namespace poppler;

        byte_array data(input_str.begin(), input_str.end());

        document *doc = nullptr;

        doc = document::load_from_data(&data);
        if (!doc)
                return "";

        auto trim = [&] (const std::string& line) -> std::string {
                const char* WhiteSpace = " \t\v\r\n";
                std::size_t start = line.find_first_not_of(WhiteSpace);
                std::size_t end = line.find_last_not_of(WhiteSpace);
                return start == end ? std::string() : line.substr(start, end - start + 1);
        };

        int numPages = doc->pages();
        std::string output = "";

        for (int i = 0; i < numPages; ++i) {
                const page *pdfPage = doc->create_page(i);
                auto text = pdfPage->text().to_latin1();
                std::replace(text.begin(), text.end(), '\n', ' ');
                std::replace(text.begin(), text.end(), '\t', ' ');
                output += trim(text);
        }

        std::cerr << output << "\n";
        return output;
}

/*
std::string file_to_text(const std::string& inputPdfPath)
{
        using namespace poppler;

        document *doc = nullptr;
        try {
                doc = document::load_from_file(inputPdfPath.c_str());

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
        } catch (const std::exception& e) {
                std::cerr << "ERROR: " << e.what() << "\n";
        }

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
*/
