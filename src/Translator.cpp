///////////////////////////////////////////////////////////////////////////////
// Translator.h
//
// This file implements the Translator class, which performs a CURL operation to
// a REST translator endpoint and returns the result.
//
///////////////////////////////////////////////////////////////////////////////

#include "../include/Translator.h"
#include <ctype.h>
#include <nlohmann/json.hpp>

// ctor
Translator::Translator()
    :   curl(curl_easy_init()),
        url("http://api.fanyi.baidu.com/api/trans/vip/translate?"),
        appId("20231114001879483"),
        key("RIFhcKg9F5Dtn6VPgmaM")
        // Another account for Ke:
        // appId("20231001001833905"),
        // key("6SSOJ03Rb9dNXrTFPaV2")
{
    if (!curl) {
        std::cerr << "Error initializing libcurl." << std::endl;
    }
}

// dtor
Translator::~Translator()
{
    curl_easy_cleanup(curl);
}

// Non-class-defined callback to store results of curl operation
// https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    readBuffer.clear();

    size_t realsize = size * nmemb;
    readBuffer.append(reinterpret_cast<char*>(contents), realsize);
    return realsize;
}

std::string url_encode(const std::string& decoded)
{
    const auto encoded_value = curl_easy_escape(nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
    std::string result(encoded_value);
    curl_free(encoded_value);
    return result;
}

// Function to perform the translation
bool Translator::doTranslation(std::string& translatedText,  // output
                               std::string textToBeTranslated,
                               std::string& fromLang,
                               std::string& toLang) const
{
    // Check that curl was initialized on construction
    if (!curl) {
        std::cerr << "Error initializing libcurl." << std::endl;
        return false;
    }

    // Change the space to '-' using std::replace_if
    std::replace_if(textToBeTranslated.begin(), textToBeTranslated.end(), [](char c) { return c == ' '; }, '-');

    std::string tbt = "";


    for (const auto& x : textToBeTranslated) {
            if (isalnum(x) || x == '-')
                    tbt.push_back(x);
    }

    tbt = url_encode(tbt);

    // Create the salt
    char salt[60];
    sprintf(salt, "%d", rand());

    // Create the sign
    std::string sign = appId + tbt + salt + key;

    // Generate an MD5 hash of the sign
    unsigned char md[16];
    char tmp[3] = {'\0'};
    char buf[33] = {'\0'};

    MD5((unsigned char*)sign.c_str(), sign.length(), md);

    for (int i = 0; i < 16; i++)
    {
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf, tmp);
    }

    // Assemble the query string
    std::ostringstream oss;
    oss << url
    << "q=" << tbt
    << "&from=" << fromLang <<
    "&to=" << toLang <<
    "&appid=" << appId
    << "&salt=" << salt <<
    "&sign=" << buf;

    // Set the URL and define the callback to store the response
    curl_easy_setopt(curl, CURLOPT_URL, oss.str().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    // Perform the HTTP GET request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        std::cerr << "TRANSLATOR: curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    // The raw response is a std::string containing JSON data.
    // The relevant field is trans_result.dst which contains the translated text.
    //
    // Parse the JSON
    nlohmann::json j = nlohmann::json::parse(readBuffer);

    // Get the translated text
    nlohmann::json trans_result = j["trans_result"][0];
    translatedText = trans_result["dst"];

    return true;
}
