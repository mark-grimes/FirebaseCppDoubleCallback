//
//  ViewController.m
//  FirebaseCppDoubleCallback
//
//  Created by Mark Grimes on 15/04/2021.
//

#import "ViewController.h"
#include "CppFirebaseCalls.hpp"

@interface ViewController ()
{
    std::unique_ptr<FirebaseClient> pFirebaseCpp_;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    // Figure out the bundle's document directory and use that to download to
    NSError* nsError;
    NSURL* documentDirectory = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory
                                                                      inDomain:NSUserDomainMask
                                                             appropriateForURL:nil
                                                                        create:true
                                                                         error:&nsError];

    std::error_code error;
    pFirebaseCpp_.reset( new FirebaseClient( error ) );
    if( !error )
    {
        pFirebaseCpp_->downloadFile( [documentDirectory fileSystemRepresentation], [](std::string path,std::error_code error) {
            if( error ) NSLog(@"There was an error downloading the file");
            else NSLog(@"File has been downloaded and saved to '%s'", path.c_str());
        } );
    }
    else NSLog(@"There was an error initialising the Firebase C++ client");
}


@end
