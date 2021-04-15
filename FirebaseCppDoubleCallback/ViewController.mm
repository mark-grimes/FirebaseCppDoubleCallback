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
    
    pFirebaseCpp_.reset( new FirebaseClient() );
    std::string result = pFirebaseCpp_->downloadFile();
    NSLog(@"File has been downloaded and saved to '%s'", result.c_str());
}


@end
