///////////////////////////////////////////////////////////////////////////////
// Translator.h
//
// This file implements the Translator class, which performs a CURL operation to
// a REST translator endpoint and returns the result.
//
///////////////////////////////////////////////////////////////////////////////

#include "../include/Translator.h"

// ctor
Translator::Translator()
    :   curl(curl_easy_init()),
        url("http://api.fanyi.baidu.com/api/trans/vip/translate?"),
        appId("20231001001833905"),
        key("6SSOJ03Rb9dNXrTFPaV2")
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
    readBuffer.append((char*)contents, realsize);
    return realsize;
}

// Function to perform the translation
bool Translator::doTranslation(std::string& translatedText, // output
                               std::string textToBeTranslated,
                               std::string fromLang,
                               std::string toLang) const
{
    // Check that curl was initialized on construction
    if (!curl) {
        std::cerr << "Error initializing libcurl." << std::endl;
        return false;
    }

    // Create the salt
    char salt[60];
    sprintf(salt, "%d", rand());

    // Create the sign
    std::string sign = appId + textToBeTranslated + salt + key;

    // Generate an MD5 hash of the sign
    unsigned char md[16];
    char tmp[3] = {'\0'};
    char buf[33] = {'\0'};

    MD5((unsigned char*)sign.c_str(), sign.length(), md);

    for (int i = 0; i < 16; i++)
    {
        sprintf(tmp,"%2.2x",md[i]);
        strcat(buf,tmp);
    }

    // Assemble the query string
    std::ostringstream oss;
    oss << url 
    << "q=" << textToBeTranslated
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
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    // Print the raw response (for debugging only; TODO delete)
    // std::cout << "Response: " << readBuffer << std::endl;

    // The raw response is a std::string containing JSON data.
    // The relevant field is trans_result.dst which contains the translated text.
    // TODO: Parse the JSON.

    // Set the output
    translatedText = readBuffer;
    return true;
}
