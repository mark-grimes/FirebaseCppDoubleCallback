//
//  CppFirebaseCalls.hpp
//  FirebaseCppDoubleCallback
//
//  Created by Mark Grimes on 15/04/2021.
//

#ifndef CppFirebaseCalls_hpp
#define CppFirebaseCalls_hpp

#include <string>
#include <memory>
#include <system_error>
//
// Forward declarations
//
namespace firebase
{
    class App;
    namespace storage
    {
        class Storage;
    }
    namespace firestore
    {
        class Firestore;
    }
}

class FirebaseClient
{
public:
    FirebaseClient( std::error_code& error );
    ~FirebaseClient();
    
    /** @brief Asks Firestore which file it needs, then downloads from Firebase Storage, returning the local filename.
     */
    void downloadFile( std::string outputDirectory, std::function<void(std::string,std::error_code)> callback );
protected:
    std::unique_ptr<::firebase::App> pFirebaseApp_;
    std::unique_ptr<::firebase::storage::Storage> pStorage_;
    std::unique_ptr<::firebase::firestore::Firestore> pFirestore_;
};

#endif /* CppFirebaseCalls_hpp */
