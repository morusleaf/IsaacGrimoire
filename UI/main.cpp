#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
//#ifdef _WIN32
//#else
//#include <Security/Authorization.h>
//#include <Security/AuthorizationTags.h>
//#endif
//#include <assert.h>

int main(int argc, char *argv[]) {
    QApplication::setSetuidAllowed(true);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//int runMain(int argc, char *argv[]) {

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}

//int authRun(char *fileName) {
//    AuthorizationItem items[1];
//    items[0].name = kAuthorizationRightExecute;
//    items[0].valueLength = 0;
//    items[0].value = NULL;
//    items[0].flags = 0;
//    AuthorizationRights rights;
//    rights.count = 1;
//    rights.items = items;
//    AuthorizationFlags flags =
//            kAuthorizationFlagDefaults |
//            kAuthorizationFlagInteractionAllowed |
//            kAuthorizationFlagExtendRights;
//    AuthorizationRef ref;
//    OSStatus ret;
//    ret = AuthorizationCreate(
//                &rights,
//                kAuthorizationEmptyEnvironment,
//                flags,
//                &ref);
//    assert(ret == errAuthorizationSuccess);
//    char *Args[] = {NULL};
//    FILE *pipe = NULL;
//    ret = AuthorizationExecuteWithPrivileges(
//                ref,
//                fileName,
//                kAuthorizationFlagDefaults,
//                Args,
//                &pipe);
//    assert(ret == errAuthorizationSuccess);
//    AuthorizationFree(ref, kAuthorizationFlagDestroyRights);
//    qDebug() << "called";
//    return 0;
//}

//int main(int argc, char *argv[])
//{
//    QApplication::setSetuidAllowed(true);
//    OSStatus ret;
//    AuthorizationRef ref;
//    ret = AuthorizationCopyPrivilegedReference(&ref, kAuthorizationFlagDefaults);
//    switch (ret) {
//    case errAuthorizationInvalidRef:
//        return authRun(argv[0]);
//    case errAuthorizationSuccess:
//        return runMain(argc, argv);
//    case errAuthorizationDenied:
//        qDebug() << "denied";
//        break;
//    default:
//        qDebug() << "unknown " << ret;
//        break;
//    }
//}
