//
//  CppFirebaseCalls.cpp
//  FirebaseCppDoubleCallback
//
//  Created by Mark Grimes on 15/04/2021.
//

#include "CppFirebaseCalls.hpp"
#include <cstdio>
#include <firebase/app.h>
#include <firebase/storage.h>
#include <firebase/firestore.h>

FirebaseClient::FirebaseClient( std::error_code& error )
{
    firebase::AppOptions options;
    // I haven't set up reading from the GoogleService-Info.plist properly, so just set these directly
    // for this example.
    options.set_app_id( SET_THIS_TO_SOMETHING_VALID );
    options.set_api_key( SET_THIS_TO_SOMETHING_VALID );
    options.set_project_id( SET_THIS_TO_SOMETHING_VALID );
    options.set_storage_bucket( SET_THIS_TO_SOMETHING_VALID );

    pFirebaseApp_.reset( firebase::App::Create( options, "my-app-name" ) );
    if( pFirebaseApp_ == nullptr )
    {
        printf( "FirebaseClient constructor: unable to create the firebase::auth::App instance\n" );
        error = std::make_error_code( std::errc::io_error ); // Don't really have a good error, just use this for now
        return;
    }

    pStorage_.reset( firebase::storage::Storage::GetInstance( pFirebaseApp_.get() ) );
    if( pStorage_ == nullptr )
    {
        
        printf( "FirebaseClient constructor: unable to create the firebase::storage::Storage instance\n" );
        error = std::make_error_code( std::errc::io_error ); // Don't really have a good error, just use this for now
        return;
    }

    pFirestore_.reset( firebase::firestore::Firestore::GetInstance( pFirebaseApp_.get() ) );
    if( pFirestore_ == nullptr )
    {
        printf( "FirebaseClient constructor: unable to create the firebase::firestore::Firestore instance\n" );
        error = std::make_error_code( std::errc::io_error ); // Don't really have a good error, just use this for now
        return;
    }
}

FirebaseClient::~FirebaseClient()
{
    // No operation. Just needs to be in a compilation unit with the firebase includes.
}

void FirebaseClient::downloadFile( std::string outputDirectory, std::function<void(std::string,std::error_code)> callback )
{
    pFirestore_->Collection("config").Get().OnCompletion(
        [ this, outputDirectory = std::move(outputDirectory), callback = std::move(callback) ]( const firebase::Future<firebase::firestore::QuerySnapshot>& future )
        {
            if( future.error() != ::firebase::firestore::Error::kErrorOk )
            {
                printf( "FirebaseClient::downloadFile - error getting collection: %s\n", future.error_message() );
                return callback( std::string(), std::make_error_code( std::errc::io_error ) );
            }
            else
            {
                // Normally we'd query the QuerySnapshot here to figure out which file to download, but since this
                // is just an example always download the same file.
                const std::string localPath = outputDirectory + "/file.txt";
                const std::string localURL = "file://" + localPath;
                const std::string firebasePath = "file.txt";

                printf( "FirebaseClient::downloadFile - downloading file from %s and saving to %s\n", firebasePath.c_str(), localURL.c_str() );
                ::firebase::storage::StorageReference fileRef = pStorage_->GetReference(firebasePath);

                if( !fileRef.is_valid() )
                {
                    printf( "FirebaseClient::downloadFile - fileRef is not valid\n" );
                    return callback( std::string(), std::make_error_code( std::errc::io_error ) );
                }

                fileRef.GetFile( localURL.c_str() ).OnCompletion( [callback = std::move(callback), localPath = std::move(localPath)]( const ::firebase::Future<size_t>& getFileFuture ) {
                    if( getFileFuture.error() != 0 )
                    {
                        callback( std::string(), std::make_error_code( std::errc::io_error ) );
                    }
                    else callback( localPath, std::error_code() );
                } );
            }
        }
    );
}
