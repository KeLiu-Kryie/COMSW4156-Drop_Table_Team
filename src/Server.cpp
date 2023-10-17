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

    // Create a new user to add translations to
    CROW_ROUTE(app,"/create_user/")([this](const crow::request& req, crow::response& res){
        std::pair<int, string> mongoRes = dbHandler.create_user();
        std::ostringstream oss;
                oss << "Output Message: " << mongoRes.second
                    << std::endl
                    << "Output Code: " << mongoRes.first
                    << std::endl;
        res.set_header("Content-Type", "text/plain");
        res.write(oss.str());
        res.end();
    });

     CROW_ROUTE(app,"/get_translation_history/")([this](const crow::request& req, crow::response& res){
        auto id = req.url_params.get("id");
        if(id){
            std::pair<int, std::string> mongoRes = dbHandler.get_translation_history(id);
            std::ostringstream oss;
                oss << "Output Message: " << mongoRes.second
                    << std::endl
                    << "Output Code: " << mongoRes.first
                    << std::endl;
            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        } else {
            res.code = 400; // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    CROW_ROUTE(app,"/delete_user/")([this](const crow::request& req, crow::response& res){
        auto id = req.url_params.get("id");
        if(id){
             std::pair<int, std::string> mongoRes = dbHandler.delete_user(id);
            std::ostringstream oss;
                oss << "Output Message: " << mongoRes.second
                    << std::endl
                    << "Output Code: " << mongoRes.first
                    << std::endl;
            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        } else {
            res.code = 400; // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });
    //Add a translation into a user given a user id
    CROW_ROUTE(app,"/post_translation/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        auto id = req.url_params.get("id");
        auto textToBeTranslated = req.url_params.get("tbt");
        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);
            TranslationOutput newTranslation = TranslationOutput(toLang, fromLang, textToBeTranslated, translatedText);  
            //TODO: Check if output is valid or not
            std::pair<int, std::string> mongoRes = dbHandler.post_translation(id, newTranslation);
            int mongoResCode = mongoRes.first;
            std::string mongoResText = mongoRes.second;
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<<mongoResText
                <<std::endl
                <<"Mongo code: "<<mongoResCode;
                
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
