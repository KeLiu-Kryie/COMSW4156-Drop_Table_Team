///////////////////////////////////////////////////////////////////////////////
// Server.cpp
//
// This file defines the Server class, which is the top-level class for the
// service we're implementing.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef INCLUDE_SERVER_H_
#define INCLUDE_SERVER_H_
// This is the only include needed for crow
#include <crow.h>
#include "TranslationOutput.h"
#include "Translations.h"
#include "DatabaseHandler.h"

// Translator header
#include "../include/Translator.h"

class Server
{
 public:
    // ctor
    Server();

    // Function to start the server
    void run(unsigned int port);

 private:
    // The app itself
    crow::SimpleApp app;

    // Translator object to do translations
    Translator translator;

    // dbHandler takes care of sending and getting data to and from
    // a mongoDB collection
    DatabaseHandler dbHandler = DatabaseHandler("config.json");
};

#endif  // INCLUDE_SERVER_H_
