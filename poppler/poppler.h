
#include <string>
#include <cstdlib>

void pdftoimage(std::string& path) {

        std::string command =   "pdftoppm -singlefile -f 1 -r 72"
                                " -jpeg -jpegopt quality=90 "
                                + path + " pset";

        std::system(command.c_str());


}

void pdftotext(std::string& path) {
        std::string command =   ""
                                ""
                                "";
}
