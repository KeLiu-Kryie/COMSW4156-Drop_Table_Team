
#include <string>
#include <cstdlib>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

void pdftoimage(std::string& path) {

        std::string command =   "pdftoppm -singlefile -f 1 -r 72"
                                " -jpeg -jpegopt quality=90 "
                                + path + " pset";

        std::system(command.c_str());

}

std::vector<std::string> tokenizer(const std::string& p_pcstStr)  {

        char delim = ' ';
        std::vector<std::string> tokens;
        std::stringstream   mySstream( p_pcstStr );
        std::string         temp;

        while( getline( mySstream, temp, delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
} 

std::string pdftotext(std::string& path) {

        namespace fs = std::filesystem;

        auto tmpf = std::tmpfile();
        int fd = fileno(tmpf);

        std::string fdstr = "/proc/self/fd/" + std::to_string(fd);

        char pathbuf[256];
        std::string out = fs::read_symlink(fs::path(fdstr));

        auto outpath = tokenizer(out)[0];

        std::cerr << outpath << "\n";
        std::cerr << path << "\n";


        std::string command =   "pdftotext -layout " + path 
                                +  " " + outpath;

        std::system(command.c_str());

        std::ifstream fin(outpath);
        std::string output = "";

        std::string cur;

        while (fin >> cur)
                output += cur;

        return output;
}
