# FirebaseCppDoubleCallback

A minimal project that shows a bug in Firebase C++ SDK v7.1.1. For the issue described at https://github.com/firebase/firebase-cpp-sdk/issues/356.
Basically, an assert gets triggered if you try to call Firebase Storage from within the handler to a Firebase Firestore call.

The project links to Firebase iOS SDK v7.5.0 (set in the `Podfile`) because that's what the C++ SDK says is required in the readme.
## Steps to build

1. Download Firebase C++ SDK v7.1.1 from https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_7.1.1.zip so that it sits in `$(PROJECT_DIR)/dependencies/firebase_cpp_sdk_7_1_1`.
2. Create a firebase project for the backend data.
3. I (couldn't/didn't have time/haven't properly tried) getting it working by loading the GoogleService-Info.plist file automatically.
So set hardcoded values from that file in `CppFirebaseCalls.cpp`, anywhere you see `SET_THIS_TO_SOMETHING_VALID`.

The bug gets triggered whether there's any data in the project backend or not, which surprised me. Probably because I don't actually
read the result of the firestore query and the bug is during the storage call.

## Results

If I run this app on device I get the following output:

```
2021-04-15 15:08:39.396806+0100 FirebaseCppDoubleCallback[87511:7124145] 7.5.0 - [Firebase/Core][I-COR000004] App with name my-app-name does not exist.
FirebaseClient::downloadFile - downloading file from file.txt and saving to file:///var/mobile/Containers/Data/Application/<my app uid>/Documents/file.txt
2021-04-15 15:08:40.154664+0100 FirebaseCppDoubleCallback[87511:7124145] ASSERT: /Users/runner/work/firebase-cpp-sdk/firebase-cpp-sdk/sdk-src/app/src/mutex.h(53): ret == 0
```

With the stacktrace:
```
(lldb) bt
* thread #3, queue = 'com.google.firebase.firestore.callback', stop reason = signal SIGABRT
    frame #0: 0x00000001ccbd4414 libsystem_kernel.dylib`__pthread_kill + 8
    frame #1: 0x00000001ea730b50 libsystem_pthread.dylib`pthread_kill + 272
    frame #2: 0x00000001a8032b74 libsystem_c.dylib`abort + 104
    frame #3: 0x00000001001e731c FirebaseCppDoubleCallback`firebase::DefaultLogCallback(firebase::LogLevel, char const*, void*) + 68
    frame #4: 0x00000001001e7430 FirebaseCppDoubleCallback`firebase::LogMessageWithCallbackV(firebase::LogLevel, char const*, char*) + 208
    frame #5: 0x00000001001e75c0 FirebaseCppDoubleCallback`firebase::LogAssert(char const*, ...) + 40
    frame #6: 0x000000010021d268 FirebaseCppDoubleCallback`firebase::Mutex::~Mutex() + 56
    frame #7: 0x0000000100219d44 FirebaseCppDoubleCallback`firebase::Mutex::~Mutex() + 32
    frame #8: 0x00000001001dec24 FirebaseCppDoubleCallback`firebase::storage::internal::StorageReferenceInternal::~StorageReferenceInternal() + 72
    frame #9: 0x00000001001deca4 FirebaseCppDoubleCallback`firebase::storage::internal::StorageReferenceInternal::~StorageReferenceInternal() + 32
    frame #10: 0x00000001001cf940 FirebaseCppDoubleCallback`firebase::storage::internal::StorageReferenceInternalCommon::DeleteInternal(firebase::storage::StorageReference*) + 72
    frame #11: 0x00000001001cfad8 FirebaseCppDoubleCallback`firebase::storage::StorageReference::~StorageReference() + 32
    frame #12: 0x00000001001cfb08 FirebaseCppDoubleCallback`firebase::storage::StorageReference::~StorageReference() + 32
  * frame #13: 0x00000001001c634c FirebaseCppDoubleCallback`FirebaseClient::downloadFile(this=0x0000000282987488, future=0x000000016fd58450)>)::$_0::operator()(firebase::Future<firebase::firestore::QuerySnapshot> const&) const at CppFirebaseCalls.cpp:89:13
    frame #14: 0x00000001001c6014 FirebaseCppDoubleCallback`decltype(__f=0x0000000282987488, __args=0x000000016fd58450)>)::$_0&>(fp)(std::__1::forward<firebase::Future<firebase::firestore::QuerySnapshot> const&>(fp0))) std::__1::__invoke<FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0&, firebase::Future<firebase::firestore::QuerySnapshot> const&>(FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0&, firebase::Future<firebase::firestore::QuerySnapshot> const&) at type_traits:3545:1
    frame #15: 0x00000001001c5fa4 FirebaseCppDoubleCallback`void std::__1::__invoke_void_return_wrapper<void>::__call<FirebaseClient::downloadFile(__args=0x0000000282987488, __args=0x000000016fd58450)>)::$_0&, firebase::Future<firebase::firestore::QuerySnapshot> const&>(FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0&, firebase::Future<firebase::firestore::QuerySnapshot> const&) at __functional_base:348:9
    frame #16: 0x00000001001c5f44 FirebaseCppDoubleCallback`std::__1::__function::__alloc_func<FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0, std::__1::allocator<FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0>, void (firebase::Future<firebase::firestore::QuerySnapshot> const&)>::operator(this=0x0000000282987488, __arg=0x000000016fd58450)(firebase::Future<firebase::firestore::QuerySnapshot> const&) at functional:1546:16
    frame #17: 0x00000001001c46e0 FirebaseCppDoubleCallback`std::__1::__function::__func<FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0, std::__1::allocator<FirebaseClient::downloadFile(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::function<void (std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::error_code)>)::$_0>, void (firebase::Future<firebase::firestore::QuerySnapshot> const&)>::operator(this=0x0000000282987480, __arg=0x000000016fd58450)(firebase::Future<firebase::firestore::QuerySnapshot> const&) at functional:1720:12
    frame #18: 0x000000010021dd64 FirebaseCppDoubleCallback`std::__1::__function::__value_func<void (firebase::FutureBase const&)>::operator()(firebase::FutureBase const&) const + 88
    frame #19: 0x000000010021dd00 FirebaseCppDoubleCallback`std::__1::function<void (firebase::FutureBase const&)>::operator()(firebase::FutureBase const&) const + 60
    frame #20: 0x000000010021b66c FirebaseCppDoubleCallback`firebase::CallStdFunction(firebase::FutureBase const&, void*) + 48
    frame #21: 0x000000010021a770 FirebaseCppDoubleCallback`firebase::ReferenceCountedFutureImpl::RunCallback(firebase::FutureBase*, void (*)(firebase::FutureBase const&, void*), void*) + 68
    frame #22: 0x000000010021a664 FirebaseCppDoubleCallback`firebase::ReferenceCountedFutureImpl::ReleaseMutexAndRunCallbacks(firebase::FutureHandle const&) + 212
    frame #23: 0x000000010034f364 FirebaseCppDoubleCallback`void firebase::ReferenceCountedFutureImpl::CompleteInternal<firebase::firestore::QuerySnapshot, void firebase::firestore::Promise<firebase::firestore::QuerySnapshot>::SetValue<firebase::firestore::QuerySnapshot, void>(firebase::firestore::QuerySnapshot)::'lambda'(firebase::firestore::QuerySnapshot*)>(firebase::FutureHandle const&, int, char const*, void const&) + 256
    frame #24: 0x000000010034f240 FirebaseCppDoubleCallback`void firebase::ReferenceCountedFutureImpl::Complete<firebase::firestore::QuerySnapshot, void firebase::firestore::Promise<firebase::firestore::QuerySnapshot>::SetValue<firebase::firestore::QuerySnapshot, void>(firebase::firestore::QuerySnapshot)::'lambda'(firebase::firestore::QuerySnapshot*)>(firebase::SafeFutureHandle<firebase::firestore::QuerySnapshot>, int, char const*, void const&) + 76
    frame #25: 0x000000010034f028 FirebaseCppDoubleCallback`void firebase::firestore::Promise<firebase::firestore::QuerySnapshot>::SetValue<firebase::firestore::QuerySnapshot, void>(firebase::firestore::QuerySnapshot) + 132
    frame #26: 0x000000010034eea0 FirebaseCppDoubleCallback`std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)::operator()(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>) + 180
    frame #27: 0x000000010034eda0 FirebaseCppDoubleCallback`decltype(std::__1::forward<firebase::firestore::api::QuerySnapshot>(fp)(std::__1::forward<firebase::firestore::QuerySnapshot>(fp0)...)) std::__1::__invoke<std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)&, firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot> >(firebase::firestore::api::QuerySnapshot&&, firebase::firestore::QuerySnapshot&&...) + 120
    frame #28: 0x000000010034ecf4 FirebaseCppDoubleCallback`void std::__1::__invoke_void_return_wrapper<void>::__call<std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)&, firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot> >(firebase::firestore::api::QuerySnapshot&&...) + 64
    frame #29: 0x000000010034eca8 FirebaseCppDoubleCallback`std::__1::__function::__alloc_func<std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>), std::__1::allocator<std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>, void (firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>::operator()(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>&&) + 64
    frame #30: 0x000000010034d4e0 FirebaseCppDoubleCallback`std::__1::__function::__func<std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>), std::__1::allocator<std::__1::unique_ptr<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>, std::__1::default_delete<firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot> > > firebase::firestore::ListenerWithPromise<firebase::firestore::api::QuerySnapshot, firebase::firestore::QuerySnapshot>(firebase::firestore::Promise<firebase::firestore::QuerySnapshot>)::'lambda'(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>, void (firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>::operator()(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>&&) + 64
    frame #31: 0x0000000100348f70 FirebaseCppDoubleCallback`std::__1::__function::__value_func<void (firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>::operator()(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>&&) const + 88
    frame #32: 0x0000000100348e80 FirebaseCppDoubleCallback`std::__1::function<void (firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>::operator()(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>) const + 52
    frame #33: 0x0000000100348c14 FirebaseCppDoubleCallback`firebase::firestore::core::EventListener<firebase::firestore::api::QuerySnapshot>::Create(std::__1::function<void (firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>)>)::CallbackEventListener::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>) + 112
    frame #34: 0x0000000100e8c18c FirebaseFirestore`firebase::firestore::api::Query::GetDocuments(this=0x0000000280abbce0, maybe_snapshot=StatusOr<firebase::firestore::api::QuerySnapshot> @ 0x000000016fd59b98)::ListenOnce::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::api::QuerySnapshot>) at query_core.cc:152:20
    frame #35: 0x0000000100e8dfe8 FirebaseFirestore`firebase::firestore::api::Query::AddSnapshotListener(this=0x0000000283199ba0, maybe_snapshot=StatusOr<firebase::firestore::core::ViewSnapshot> @ 0x000000016fd5a550)::Converter::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>) at query_core.cc:201:23
    frame #36: 0x0000000100b7c34c FirebaseFirestore`firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(this=0x0000000107d04d98)::'lambda'()::operator()() const at event_listener.h:146:31
    frame #37: 0x0000000100b7c284 FirebaseFirestore`decltype(__f=0x0000000107d04d98)(std::__1::forward<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'()&>(fp0)...)) std::__1::__invoke<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'()&>(firebase::firestore::core::ViewSnapshot&&, firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'()&...) at type_traits:3545:1
    frame #38: 0x0000000100b7c238 FirebaseFirestore`void std::__1::__invoke_void_return_wrapper<void>::__call<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(__args=0x0000000107d04d98)::'lambda'()&>(firebase::firestore::core::ViewSnapshot&&...) at __functional_base:348:9
    frame #39: 0x0000000100b7c210 FirebaseFirestore`std::__1::__function::__alloc_func<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'(), std::__1::allocator<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'()>, void ()>::operator(this=0x0000000107d04d98)() at functional:1546:16
    frame #40: 0x0000000100b7a930 FirebaseFirestore`std::__1::__function::__func<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'(), std::__1::allocator<firebase::firestore::core::AsyncEventListener<firebase::firestore::core::ViewSnapshot>::OnEvent(firebase::firestore::util::StatusOr<firebase::firestore::core::ViewSnapshot>)::'lambda'()>, void ()>::operator(this=0x0000000107d04d90)() at functional:1720:12
    frame #41: 0x000000010032dadc FirebaseCppDoubleCallback`std::__1::__function::__value_func<void ()>::operator()() const + 60
    frame #42: 0x000000010032da94 FirebaseCppDoubleCallback`std::__1::function<void ()>::operator()() const + 24
    frame #43: 0x0000000100f72290 FirebaseFirestore`firebase::firestore::util::Task::ExecuteAndRelease(this=0x0000000283f8c480) at task.cc:102:9
    frame #44: 0x0000000100bad8a4 FirebaseFirestore`firebase::firestore::util::ExecutorLibdispatch::InvokeAsync(raw_task=0x0000000283f8c480) at executor_libdispatch.mm:237:9
    frame #45: 0x00000001044396c0 libdispatch.dylib`_dispatch_client_callout + 20
    frame #46: 0x0000000104441354 libdispatch.dylib`_dispatch_lane_serial_drain + 736
    frame #47: 0x00000001044420c0 libdispatch.dylib`_dispatch_lane_invoke + 448
    frame #48: 0x000000010444e644 libdispatch.dylib`_dispatch_workloop_worker_thread + 1520
    frame #49: 0x00000001ea731814 libsystem_pthread.dylib`_pthread_wqthread + 276
    ```
