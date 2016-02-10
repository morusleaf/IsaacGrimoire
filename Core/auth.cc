#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <string.h>
#include "util.h"

// Run the plugin as privileged process since we need to read other process
// Reference: http://www.occam.com/osx/OSX_SecFmwk.pdf
// Reference: https://developer.apple.com/library/mac/documentation/Security/Reference/SecurityFrameworkReference/index.html
int main(int argc, char *argv[]) {
    char fileName[1024];
    strcpy(fileName, argv[0]);
    char *pch = fileName;
    while (*pch != '\0') pch++;
    while (*pch != '/') pch--;
    pch++;
    strcpy(pch, "IsaacGrimoire");
    AuthorizationItem right = {kAuthorizationRightExecute, 0, NULL, 0};
    AuthorizationRights rightSet = {1, &right};
    AuthorizationFlags flags = kAuthorizationFlagDefaults |
        kAuthorizationFlagInteractionAllowed |
        kAuthorizationFlagExtendRights;
    AuthorizationRef ref;
    OSStatus status = AuthorizationCreate(&rightSet,
            kAuthorizationEmptyEnvironment, flags, &ref);
    if (status != errAuthorizationSuccess)
        return -1;
    /* ALERT("Authorization created"); */
    char *GrimoireArgs[] = {NULL};
    FILE *pipe = NULL;
    status = AuthorizationExecuteWithPrivileges(ref, fileName,
            kAuthorizationFlagDefaults, GrimoireArgs, &pipe);
    /* char buffer[1024]; */
    /* sprintf(buffer, "Authorization execute status: %d", status); */
    /* ALERT(buffer); */
    if (status != errAuthorizationSuccess)
        return -1;
    AuthorizationFree(ref, kAuthorizationFlagDestroyRights);
    return 0;
}
