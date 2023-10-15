///////////////////////////////////////////////////////////////////////////////
// Server.cpp
//
// This file defines the Server class, which is the top-level class for the
// service we're implementing.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SERVER_H
#define SERVER_H

// This is the only include needed for crow
#include "crow.h"
#include "DatabaseHandler.h"

// Translator header
#include "../include/Translator.h"

class Server
{
public:
    // ctor
    Server();

    // Function to start the server
    void run();

private:
    // The app itself
    crow::SimpleApp app;

    // Translator object to do translations
    Translator translator;

    //dbHandler takes care of sending and getting data to and from
    //a mongoDB collection
    DatabaseHandler dbHandler = DatabaseHandler();
    
};

#endif // SERVER_H
