///////////////////////////////////////////////////////////////////////////////
// Translator.h
//
// This file defines the Translator class, which performs a CURL operation to
// a REST translator endpoint and returns the result.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <curl/curl.h>
#include <iostream>
#include <openssl/md5.h>
#include <sstream>
#include <string>
#include <cstring>

// Non-class defined static string to store the translated result
// after performing the curl operation
static std::string readBuffer;

class Translator
{
public:
    // ctor
    Translator();

    // dtor
    ~Translator();

    // Function to perform the translation.
    //
    // Inputs:
    //          textToBeTranslated - the input text
    //          fromLang - the language that textToBeTranslated is in
    //          toLang - the language to translate to
    // Outputs:
    //          translatedText - the translated text
    //          bool - indicates success (true) or failure (false)
    bool doTranslation(std::string& translatedText, // output
                       std::string textToBeTranslated,
                       std::string fromLang,
                       std::string toLang) const;

private:
    // Fundamental curl pointer
    CURL *curl;

    // REST endpoint
    std::string url;

    // Not exactly sure what this field is for.
    // @Ke -- any idea?
    std::string appId;

    // API Key
    std::string key;
};

#endif // TRANSLATOR_H