//
//  CppFirebaseCalls.hpp
//  FirebaseCppDoubleCallback
//
//  Created by Mark Grimes on 15/04/2021.
//

#ifndef CppFirebaseCalls_hpp
#define CppFirebaseCalls_hpp

#include <string>

/** @brief Asks Firestore which file it needs, then downloads from Firebase Storage, returning the local filename.
 */
std::string downloadFile();

#endif /* CppFirebaseCalls_hpp */
