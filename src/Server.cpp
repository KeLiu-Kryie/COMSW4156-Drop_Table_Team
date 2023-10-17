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

//     //define route to store translations onto a mongodb collection
//     CROW_ROUTE(app,"/post_translation/")([this](const crow::request& req, crow::response& res){
//         // Access query string parameters using crow::request::url_params
//         auto textToBeTranslated = req.url_params.get("tbt");
//         auto toLang = req.url_params.get("tl");
//         auto fromLang = req.url_params.get("fl");
//         std::replace( uri_str.begin(), uri_str.end(), ' ', '+');

//         // Check if parameters exist and handle accordingly
//         if (textToBeTranslated && toLang && fromLang)
//         {
//             // Output
//             std::string translatedText;
//             translator.doTranslation(translatedText, textToBeTranslated, toLang, fromLang);
//             //TODO: Check if output is valid or not
//             std::pair<int, std::string> mongoRes = dbHandler.post(translatedText);
//             std::string result_id = mongoRes.second;
//             // The rest of this IF block is for debugging
//             std::ostringstream oss;
//             oss << "textToBeTranslated: " << textToBeTranslated
//                 << ", toLang: " << toLang
//                 << ", fromLang: " << fromLang
//                 << std::endl
//                 << "Translated text: " << translatedText
//                 << std::endl
//                 <<"Mongo res: "<<result_id;
                
//             res.set_header("Content-Type", "text/plain");
//             res.write(oss.str());
//             res.end();
//         }
//         else
//         {
//             res.code = 400; // Bad Request
//             res.write("Missing query parameters.\n");
//             res.end();
//         }
//     });
// }

// Function to start the server
void Server::run()
{
    app.port(18080).multithreaded().run();
}
