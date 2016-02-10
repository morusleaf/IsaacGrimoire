#include "mainwindow.h"
#include <QApplication>
#ifdef _WIN32
#else
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <string.h>
#endif

#ifdef _WIN32
#else
// Run the plugin as privileged process since we need to read other process
// Reference: http://www.occam.com/osx/OSX_SecFmwk.pdf
// Reference: https://developer.apple.com/library/mac/documentation/Security/Reference/SecurityFrameworkReference/index.html
int authRun(char* fileName) {
    AuthorizationItem right = {kAuthorizationRightExecute, 0, NULL, 0};
    AuthorizationRights rightSet = {1, &right};
    AuthorizationFlags flags = kAuthorizationFlagDefaults
            | kAuthorizationFlagInteractionAllowed
            | kAuthorizationFlagExtendRights;
    AuthorizationRef ref;
    OSStatus status = AuthorizationCreate(
                &rightSet,
                kAuthorizationEmptyEnvironment,
                flags,
                &ref);
    if (status != errAuthorizationSuccess)
        return -1;
    char *Args[] = {"withprivilege", NULL};
    FILE *pipe = NULL;
    status = AuthorizationExecuteWithPrivileges(
                ref,
                fileName,
                kAuthorizationFlagDefaults,
                Args,
                &pipe);
    if (status != errAuthorizationSuccess)
        return -1;
    AuthorizationFree(ref, kAuthorizationFlagDestroyRights);
    return 0;
}
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#else
    QApplication::setSetuidAllowed(true);
    if (argc == 2 && strcmp(argv[1], "withprivilege") == 0) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
        return authRun(argv[0]);
#endif
}
