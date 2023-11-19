///////////////////////////////////////////////////////////////////////////////
// Server.cpp
//
// This file implements the Server class, which is the top-level class for the
// service we're implementing.
//
///////////////////////////////////////////////////////////////////////////////

#include "nlohmann/json.hpp"

#include "../include/Server.h"
#include "../include/ocr.h"
#include "../include/pdftotext.h"

// ctor
Server::Server()
    : app(),
      translator()
{
    ///////////////////////////////////////////////////////////////////////////
    //  Route: /translate/
    //
    //  Function: Provide base translation functionality.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/translate/")([this](const crow::request& req, crow::response& res)
    {
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

            // Create a JSON object
            nlohmann::json j;
            j["textToBeTranslated"] = textToBeTranslated;
            j["toLang"] = toLang;
            j["fromLang"] = fromLang;
            j["translatedText"] = translatedText;

            // Convert the JSON object to a string and return it
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /create_client/
    //
    //  Function: Create a new MongoDB ID string to associate translations to.
    //            The response returns the id of the user if successful.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/create_client/").methods("POST"_method)([this](const crow::request& req, crow::response& res)
    {
        std::pair<int, std::string> mongoRes = dbHandler.create_client();

        // Construct a json object
        nlohmann::json j;
        j["message"] = mongoRes.second;
        j["code"] = mongoRes.first;

        // Write response
        res.write(j.dump());
        res.end();
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /get_translation_history/
    //
    //  Function: Given a valid user ID (returned from the /create_client endpoint),
    //            Return the translation history as a json string.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/get_translation_history/")([this](const crow::request& req, crow::response& res)
    {
        auto id = req.url_params.get("id");
        if (id) {
            std::pair<int, std::string> mongoRes = dbHandler.get_translation_history(id);

            // Construct a json object
            nlohmann::json j;
            j["message"] = mongoRes.second;
            j["code"] = mongoRes.first;

            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /delete_client/
    //
    //  Function: Delete the passed-in user ID and any data associated with it.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/delete_client/").methods("DELETE"_method)([this](const crow::request& req, crow::response& res)
    {
        auto id = req.url_params.get("id");
        if (id) {
            std::pair<int, std::string> mongoRes = dbHandler.delete_client(id);

            // Construct a json object
            nlohmann::json j;
            j["message"] = mongoRes.second;
            j["code"] = mongoRes.first;

            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /post_translation_to_client/
    //
    //  Function: Store a translation into the datastore and
    //            associate it with the given user ID.
    ///////////////////////////////////////////////////////////////////////////
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


            // Construct a json object
            nlohmann::json j;
            j["textToBeTranslated"] = textToBeTranslated;
            j["toLang"] = toLang;
            j["fromLang"] = fromLang;
            j["translatedText"] = translatedText;
            j["mongoRes"] = mongoResText;
            j["mongoCode"] = mongoResCode;


            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /pdf_to_text/
    //
    //  Function: Read a passed-in PDF file and return the text output.
    //            This endpoint does not perform translation, only OCR.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/pdf_to_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params

        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        // Check if parameters exist and handle accordingly
        if (pdf.size())
        {
            // Output
            try
            {
                std::string text = data_to_text(pdf);

                if (text.size() == 0) {
                    res.code = 400;
                    res.set_header("Content-Type", "text/plain");
                    res.write("Invalid PDF file");
                    res.end();
                }

                // Construct a json object
                nlohmann::json j;
                j["text"] = text;

                // Write response
                res.write(j.dump());
            }
            catch (const std::exception& e)
            {
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

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /translate_pdf_text/
    //
    //  Function: Read a passed-in PDF file, perform translation,
    //            and return the text output.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/translate_pdf_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");
        std::string textToBeTranslated = data_to_text(pdf);

        if (textToBeTranslated.size() == 0)
        {
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

            // Construct a json object
            nlohmann::json j;
            j["textToBeTranslated"] = textToBeTranslated;
            j["toLang"] = toLang;
            j["fromLang"] = fromLang;
            j["translatedText"] = translatedText;

            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /post_pdf_translation/
    //
    //  Function: Read a passed-in PDF file, perform translation,
    //            store the result in the datastore, and return the text output.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/post_pdf_translation/").methods("POST"_method)([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto pdf = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto id = req.url_params.get("id");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = data_to_text(pdf);

        if (textToBeTranslated.size() == 0)
        {
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

            // Construct a json object
            nlohmann::json j;
            j["textToBeTranslated"] = textToBeTranslated;
            j["toLang"] = toLang;
            j["fromLang"] = fromLang;
            j["translatedText"] = translatedText;
            j["mongoRes"] = mongoRes.second;

            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /image_to_text/
    //
    //  Function: Read a passed-in image file and return the text output.
    ///////////////////////////////////////////////////////////////////////////
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

            if (text.size() == 0)
            {
                    res.code = 400;
                    res.set_header("Content-Type", "text/plain");
                    res.write("Invalid file");
                    res.end();
            }

            // Construct a json object
            nlohmann::json j;
            j["text"] = text;

            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.set_header("Content-Type", "text/plain");
            res.write("Missing query parameters.\n");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /translate_image_text/
    //
    //  Function: Read a passed-in image file, perform translation,
    //            and return the text output.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/translate_image_text/")([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;

        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = ocr::image_to_text(image, "eng");

        if (textToBeTranslated.size() == 0)
        {
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

            // Construct a json object
            nlohmann::json j;
            j["textToBeTranslated"] = textToBeTranslated;
            j["toLang"] = toLang;
            j["fromLang"] = fromLang;
            j["translatedText"] = translatedText;

            // Write response
            res.write(j.dump());
            res.end();
        }
        else
        {
            res.code = 400;  // Bad Request
            res.write("Missing query parameters.\n");
            res.set_header("Content-Type", "text/plain");
            res.end();
        }
    });

    ///////////////////////////////////////////////////////////////////////////
    //  Route: /post_image_translation/
    //
    //  Function: Read a passed-in image file, perform translation,
    //            store the result in the datastore, and return the text output.
    ///////////////////////////////////////////////////////////////////////////
    CROW_ROUTE(app, "/post_image_translation/").methods("POST"_method)([this](const crow::request& req, crow::response& res){
        // Access query string parameters using crow::request::url_params
        crow::multipart::message msg(req);
        auto image = msg.get_part_by_name("file").body;
        auto id = req.url_params.get("id");
        auto toLang = req.url_params.get("tl");
        auto fromLang = req.url_params.get("fl");

        auto textToBeTranslated = ocr::image_to_text(image, "eng");

        if (textToBeTranslated.size() == 0)
        {
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

            // Construct a json object
            nlohmann::json j;
            j["textToBeTranslated"] = textToBeTranslated;
            j["toLang"] = toLang;
            j["fromLang"] = fromLang;
            j["translatedText"] = translatedText;
            j["mongoRes"] = mongoRes.second;

            // Write response
            res.write(j.dump());
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
void Server::run(unsigned int port)
{
    app.port(port).multithreaded().run();
}
