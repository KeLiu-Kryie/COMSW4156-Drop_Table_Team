#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::string performocr(std::string& input, std::string& language) {

        using namespace tesseract;
        using namespace cv;

        std::vector<uchar> bytes(input.begin(), input.end());

        auto image = imdecode(bytes, 0);

        auto ocr = TessBaseAPI();

        ocr.Init(NULL, language.c_str(), OEM_LSTM_ONLY);

        ocr.SetPageSegMode(PSM_AUTO);

        ocr.SetImage(image.data, image.cols, image.rows, 3, image.step);

        std::string output = ocr.GetUTF8Text();

        ocr.End();

        return output;
}
/*
int main()
{
        std::ifstream fin("test.png", std::ios::binary);

        std::ostringstream ss;
        ss << fin.rdbuf();

        std::string png = ss.str();

        fin.close();
        std::string lang = "eng";

        std::cout << performocr(png, lang) << "\n";

}
*/
