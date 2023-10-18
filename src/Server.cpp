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

    //define route to store translations onto a mongodb collection
    CROW_ROUTE(app,"/post_translation/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        auto textToBeTranslated = req.url_params.get("tbt");
        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");
        std::string uri_str = req.url_params.get("uri");
        auto dbName = req.url_params.get("db");
        std::replace( uri_str.begin(), uri_str.end(), ' ', '+');

        dbHandler.setupCollectionEndpoint(uri_str, dbName);

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);
            //TODO: Check if output is valid or not
            std::string mongoRes = dbHandler.post(translatedText);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<<mongoRes;
                
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

    //define route to store translations onto a mongodb collection
    CROW_ROUTE(app,"/post_pdf_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        std::string uri_str = req.url_params.get("uri");
        auto dbName = req.url_params.get("db");
        std::replace( uri_str.begin(), uri_str.end(), ' ', '+');

        dbHandler.setupCollectionEndpoint(uri_str, dbName);

        // Check if parameters exist and handle accordingly
        if (pdf.size())
        {
            // Output
            std::string text = data_to_text(pdf);

            //TODO: Check if output is valid or not

            std::string mongoRes = dbHandler.post(text);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "text: " << text
                << std::endl
                <<"Mongo res: "<< mongoRes;
                
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
            // translator.doTranslation(translatedText, "hello", toLang, fromLang);
            translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);

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
        auto fromLang = req.url_params.get("fl");
        std::string uri_str = req.url_params.get("uri");
        auto dbName = req.url_params.get("db");
        std::replace( uri_str.begin(), uri_str.end(), ' ', '+');

        dbHandler.setupCollectionEndpoint(uri_str, dbName);

        auto textToBeTranslated = data_to_text(pdf);

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);

            //TODO: Check if output is valid or not
            std::string mongoRes = dbHandler.post(translatedText);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<<mongoRes;
                
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

    //define route to store translations onto a mongodb collection
    CROW_ROUTE(app,"/post_image_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        std::string uri_str = req.url_params.get("uri");
        auto dbName = req.url_params.get("db");
        std::replace( uri_str.begin(), uri_str.end(), ' ', '+');

        dbHandler.setupCollectionEndpoint(uri_str, dbName);

        // Check if parameters exist and handle accordingly
        if (image.size())
        {
            // Output
            std::string text = ocr::image_to_text(image, "eng");

            //TODO: Check if output is valid or not

            std::string mongoRes = dbHandler.post(text);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "text: " << text
                << std::endl
                <<"Mongo res: "<< mongoRes;
                
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

    //define route to store translations onto a mongodb collection
    CROW_ROUTE(app,"/post_image_translation/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        std::string uri_str = req.url_params.get("uri");
        auto dbName = req.url_params.get("db");
        std::replace( uri_str.begin(), uri_str.end(), ' ', '+');

        dbHandler.setupCollectionEndpoint(uri_str, dbName);

        auto textToBeTranslated = ocr::image_to_text(image, fromLang);

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);

            //TODO: Check if output is valid or not
            std::string mongoRes = dbHandler.post(translatedText);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<<mongoRes;
                
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
