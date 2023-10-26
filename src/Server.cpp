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
    CROW_ROUTE(app, "/translate/")([this](const crow::request& req, crow::response& res){
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
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    // Create a new user to add translations to
    // Response returns id of user if successful
    CROW_ROUTE(app, "/create_client/").methods("POST"_method)([this](const crow::request& req, crow::response& res){
        std::pair<int, std::string> mongoRes = dbHandler.create_client();
        std::ostringstream oss;
                oss << "Output Message: " << mongoRes.second
                    << std::endl
                    << "Output Code: " << mongoRes.first
                    << std::endl;
        res.set_header("Content-Type", "text/plain");
        res.write(oss.str());
        res.end();
    });

     // Given a valid user id (returned from create user)
     // Return the translation history as a json string
     CROW_ROUTE(app, "/get_translation_history/")([this](const crow::request& req, crow::response& res){
        auto id = req.url_params.get("id");
        if (id) {
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
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });
    // Delete a given user id
    CROW_ROUTE(app, "/delete_client/").methods("DELETE"_method)([this](const crow::request& req, crow::response& res){
        auto id = req.url_params.get("id");
        if (id) {
             std::pair<int, std::string> mongoRes = dbHandler.delete_client(id);
            std::ostringstream oss;
                oss << "Output Message: " << mongoRes.second
                    << std::endl
                    << "Output Code: " << mongoRes.first
                    << std::endl;
            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        } else {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });
    // Add a translation into a user given a user id
    // Translation output is stored in mongodb
    CROW_ROUTE(app, "/post_translation_to_client/").methods("POST"_method)([this](const crow::request& req, crow::response& res){
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
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);
            TranslationOutput newTranslation = TranslationOutput(toLang, fromLang, textToBeTranslated, translatedText);
            // TODO: Check if output is valid or not
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
                << "Mongo res: "<< mongoResText
                << std::endl
                << "Mongo code: "<< mongoResCode;

            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    // Read a pdf file and return text output
    CROW_ROUTE(app, "/pdf_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params

        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        // Check if parameters exist and handle accordingly
        if (pdf.size())
        {
            // Output

                try {
                    std::string text = data_to_text(pdf);

                    if (text.size() == 0) {
                        res.code = 400;
                        res.set_header("Content-Type", "text/plain");
                        res.write("Invalid PDF file");
                        res.end();
                    }
                    // The rest of this IF block is for debugging
                    std::ostringstream oss;
                    oss << "text: " << text << std::endl;

                    res.code = 200;
                    res.set_header("Content-Type", "text/plain");
                    res.write(oss.str());
                    res.end();

                } catch (const std::exception& e) {
                        res.code = 400;
                        res.set_header("Content-Type", "text/plain");
                        res.write("Invalid PDF file");
                        res.end();
                }
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing Query Parameters.\n");
            res.end();
        }
    });
    // Take a pdf file and return the text in a different language
    CROW_ROUTE(app, "/translate_pdf_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");
        std::string textToBeTranslated = data_to_text(pdf);

        if (textToBeTranslated.size() == 0) {
                res.code = 400;
                res.set_header("Content-Type", "text/plain");
                res.write("Invalid PDF file");
                res.end();
        }

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
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    // Take a pdf file and return the text in a different language
    // Store translation output into id provided
    CROW_ROUTE(app, "/post_pdf_translation/").methods("POST"_method)([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto id = req.url_params.get("id");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = data_to_text(pdf);

        if (textToBeTranslated.size() == 0) {
                res.code = 400;
                res.set_header("Content-Type", "text/plain");
                res.write("Invalid PDF file");
                res.end();
        }

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);
            TranslationOutput newTranslation = TranslationOutput(toLang, fromLang, textToBeTranslated, translatedText);
            // TODO: Check if output is valid or not
            std::pair<int, std::string> mongoRes = dbHandler.post_translation(id, newTranslation);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<< mongoRes.first;

            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    // Take in image and return string text present in image
    CROW_ROUTE(app, "/image_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        // Check if parameters exist and handle accordingly
        if (image.size())
        {
            // Output
            std::string text = ocr::image_to_text(image, "eng");

                if (text.size() == 0) {
                        res.code = 400;
                        res.set_header("Content-Type", "text/plain");
                        res.write("Invalid file");
                        res.end();
                }

            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "text: " << text << std::endl;

            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    // Take in image and return the translation output of the text in the image
    CROW_ROUTE(app, "/translate_image_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = ocr::image_to_text(image, "eng");

        if (textToBeTranslated.size() == 0) {
                res.code = 400;
                res.set_header("Content-Type", "text/plain");
                res.write("Invalid file");
                res.end();
        }

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
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    // Take in image and translate the text
    // store translation output into mongodb
    CROW_ROUTE(app, "/post_image_translation/").methods("POST"_method)([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;
        auto id = req.url_params.get("id");
        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = ocr::image_to_text(image, "eng");

        if (textToBeTranslated.size() == 0) {
                res.code = 400;
                res.set_header("Content-Type", "text/plain");
                res.write("Invalid file");
                res.end();
        }

        // Check if parameters exist and handle accordingly
        if (textToBeTranslated.size() && toLang && fromLang)
        {
            // Output
            std::string translatedText;
            translator.doTranslation(translatedText, textToBeTranslated, fromLang, toLang);
            TranslationOutput newTranslation = TranslationOutput(toLang, fromLang, textToBeTranslated, translatedText);
            // TODO: Check if output is valid or not
            std::pair<int, std::string> mongoRes = dbHandler.post_translation(id, newTranslation);
            // The rest of this IF block is for debugging
            std::ostringstream oss;
            oss << "textToBeTranslated: " << textToBeTranslated
                << ", toLang: " << toLang
                << ", fromLang: " << fromLang
                << std::endl
                << "Translated text: " << translatedText
                << std::endl
                <<"Mongo res: "<< mongoRes.first;

            res.set_header("Content-Type", "text/plain");
            res.write(oss.str());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
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
