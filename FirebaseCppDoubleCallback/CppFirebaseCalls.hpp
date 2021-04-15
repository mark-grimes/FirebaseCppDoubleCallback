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
    FirebaseClient();
    ~FirebaseClient();
    
    /** @brief Asks Firestore which file it needs, then downloads from Firebase Storage, returning the local filename.
     */
    std::string downloadFile();
protected:
    std::unique_ptr<::firebase::App> pFirebaseApp_;
    std::unique_ptr<::firebase::storage::Storage> pStorage_;
    std::unique_ptr<::firebase::firestore::Firestore> pFirestore_;
};

#endif /* CppFirebaseCalls_hpp */
