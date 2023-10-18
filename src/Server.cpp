///////////////////////////////////////////////////////////////////////////////
// Server.cpp
//
// This file implements the Server class, which is the top-level class for the
// service we're implementing.
//
///////////////////////////////////////////////////////////////////////////////

#include "../include/Server.h"
#include "../include/ocr.h"
#include "../include/pdftotext.h"

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
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);

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
        std::pair<int, std::string> mongoRes = dbHandler.create_user();
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
    CROW_ROUTE(app,"/post_translation_to_user/")([this](const crow::request& req, crow::response& res){
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

    // Define the translation route
    CROW_ROUTE(app,"/pdf_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params

        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        // Check if parameters exist and handle accordingly
        if (pdf.size())
        {
            // Output
            std::string text = data_to_text(pdf);

            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "text: " << text << std::endl;
                
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

    CROW_ROUTE(app,"/translate_pdf_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");
        std::string textToBeTranslated = data_to_text(pdf);

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);

            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "toLang: " << toLang
                << ", fromLang: " << fromLang
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

    //define route to store translations onto a mongodb collection
    CROW_ROUTE(app,"/post_pdf_translation/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto id = req.url_params.get("id");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = data_to_text(pdf);

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);
            TranslationOutput newTranslation = TranslationOutput(toLang, fromLang, textToBeTranslated, translatedText);  
            //TODO: Check if output is valid or not
            std::pair<int, std::string> mongoRes = dbHandler.post_translation(id, newTranslation);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<<mongoRes.first;
                
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

    // Define the translation route
    CROW_ROUTE(app,"/image_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params

        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        // Check if parameters exist and handle accordingly
        if (image.size())
        {
            // Output
            std::string text = ocr::image_to_text(image, "eng");

            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "text: " << text << std::endl;
                
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

    CROW_ROUTE(app,"/translate_image_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = ocr::image_to_text(image, "eng");

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);

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

    //define route to store translations onto a mongodb collection
    CROW_ROUTE(app,"/post_image_translation/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;
        auto id = req.url_params.get("id");
        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = ocr::image_to_text(image, fromLang);

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);
            TranslationOutput newTranslation = TranslationOutput(toLang, fromLang, textToBeTranslated, translatedText);  
            //TODO: Check if output is valid or not
            std::pair<int, std::string> mongoRes = dbHandler.post_translation(id, newTranslation);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<<mongoRes.first;
                
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