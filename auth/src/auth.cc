#include <Security/Authorization.h>
#include <Security/AuthorizationDB.h>
#include <Security/AuthorizationTags.h>
#include <CoreFoundation/CFDictionary.h>
#include <string.h>
#include <assert.h>
#define RIGHT_NAME "com.MorusLeaf.IsaacGrimoire.readMemory"

// Run the plugin as privileged process since we need to read other process
// Reference: http://www.occam.com/osx/OSX_SecFmwk.pdf
// Reference: https://developer.apple.com/library/mac/documentation/Security/Reference/SecurityFrameworkReference/index.html
int main(int argc, char *argv[]) {
    /* OSStatus ret; */


    /* AuthorizationRef ref; */
    /* ret = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, */
    /*         kAuthorizationFlagDefaults, &ref); */
    /* assert(ret == errAuthorizationSuccess); */

    /* ret = AuthorizationRightSet( */
    /*         ref, */
    /*         RIGHT_NAME, */
    /*         CFSTR(kAuthorizationRuleClassAllow), */
    /*         CFSTR("Authorize reading memory"), */
    /*         NULL, */
    /*         NULL); */
    /* assert(ret == errAuthorizationSuccess); */




    /* AuthorizationRef ref; */
    /* ret = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, */
    /*         kAuthorizationFlagDefaults, &ref); */
    /* assert(ret == errAuthorizationSuccess); */
    /* AuthorizationItem items[1]; */
    /* items[0].name = "com.MorusLeaf.IsaacGrimoire.accessMemory"; */
    /* items[0].valueLength = 0; */
    /* items[0].value = NULL; */
    /* items[0].flags = 0; */
    /* AuthorizationRights rights; */
    /* rights.count = 1; */
    /* rights.items = items; */
    /* AuthorizationFlags flags = kAuthorizationFlagDefaults */
    /*     | kAuthorizationFlagInteractionAllowed */
    /*     | kAuthorizationFlagExtendRights; */
    /* ret = AuthorizationCopyRights(ref, &rights, */
    /*         kAuthorizationEmptyEnvironment, flags, NULL); */
    /* assert(ret == errAuthorizationSuccess); */

    /* ret = AuthorizationFree(ref, kAuthorizationFlagDestroyRights); */
    /* assert(ret == errAuthorizationSuccess); */



    /* CFDictionaryRef *rights; */
    /* ret = AuthorizationRightGet(RIGHT_NAME, NULL); */
    /* printf("%d\n", ret); */
    /* if (ret == errAuthorizationDenied) { */
    /*     ret = AuthorizationRightSet(NULL, */
    /*             RIGHT_NAME, */
    /*             CFSTR(kAuthorizationRuleClassAllow), */
    /*             CFSTR("Authorize reading memory"), */
    /*             NULL, */
    /*             NULL); */
    /*     printf("%d\n", ret); */
    /* } */



    char fileName[1024];
    strcpy(fileName, argv[0]);
    char *pch = fileName;
    while (*pch != '\0') pch++;
    while (*pch != '/') pch--;
    pch++;
    strcpy(pch, "IsaacGrimoire");
    /* char* fileName = "/Users/sangyy/Codes/build-IsaacGrimoire-Desktop_Qt_5_5_1_clang_64bit-Debug/Contents/MacOS/main"; */
    /* char* fileName = "/Users/sangyy/test/cpp/main"; */
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
    assert(status == errAuthorizationSuccess);
    char *Args[] = {NULL};
    FILE *pipe = NULL;
    status = AuthorizationExecuteWithPrivileges(
            ref,
            fileName,
            kAuthorizationFlagDefaults,
            Args,
            &pipe);
    assert(status == errAuthorizationSuccess);
    AuthorizationFree(ref, kAuthorizationFlagDestroyRights);
    return 0;
}
