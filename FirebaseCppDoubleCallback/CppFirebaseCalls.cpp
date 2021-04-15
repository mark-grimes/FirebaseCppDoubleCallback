//
//  CppFirebaseCalls.cpp
//  FirebaseCppDoubleCallback
//
//  Created by Mark Grimes on 15/04/2021.
//

#include "CppFirebaseCalls.hpp"
#include <firebase/app.h>
#include <firebase/storage.h>
#include <firebase/firestore.h>

FirebaseClient::FirebaseClient()
{
    ::firebase::AppOptions options;
    pFirebaseApp_.reset( firebase::App::Create( options, "my-app-name" ) );
}

FirebaseClient::~FirebaseClient()
{
    // No operation. Just needs to be in a compilation unit with the firebase includes.
}

std::string FirebaseClient::downloadFile()
{
    return "foobar";
}
