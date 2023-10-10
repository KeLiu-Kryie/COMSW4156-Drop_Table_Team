///////////////////////////////////////////////////////////////////////////////
// Server.cpp
//
// This file implements the Server class, which is the top-level class for the
// service we're implementing.
//
///////////////////////////////////////////////////////////////////////////////

#include "../include/Server.h"

// ctor
Server::Server()
    : app(),
      translator()
{
    // Define the translation route
    CROW_ROUTE(app,"/translate/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        auto textToBeTranslated = req.url_params.get("tbt");
        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);

            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl;
                
            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        }
        else
        {
            res.code = 400; // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });
}

// Function to start the server
void Server::run()
{
    app.port(18080).multithreaded().run();
}
