/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: AppMain.cpp
 *			Purpose: Main Application entrypoint for the Mac.
 *
 * =====================================================================
 */

#ifdef __APPLE__

#include <objc/runtime.h>
#include <objc/message.h>

#define cls objc_getClass
#define sel sel_getUid
#define msg ((id (*)(id, SEL, ...))objc_msgSend)
#define cls_msg ((id (*)(Class, SEL, ...))objc_msgSend)

// poor man's bindings!
typedef enum NSApplicationActivationPolicy {
    NSApplicationActivationPolicyRegular   = 0,
    NSApplicationActivationPolicyAccessory = 1,
    NSApplicationActivationPolicyERROR     = 2,
} NSApplicationActivationPolicy;

typedef enum NSWindowStyleMask {
    NSWindowStyleMaskBorderless     = 0,
    NSWindowStyleMaskTitled         = 1 << 0,
    NSWindowStyleMaskClosable       = 1 << 1,
    NSWindowStyleMaskMiniaturizable = 1 << 2,
    NSWindowStyleMaskResizable      = 1 << 3,
} NSWindowStyleMask;

typedef enum NSBackingStoreType {
    NSBackingStoreBuffered = 2,
} NSBackingStoreType;

void XPXMacInitStuff()
{
    // id app = [NSApplication sharedApplication];
    id app = cls_msg(cls("NSApplication"), sel("sharedApplication"));

    // [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    msg(app, sel("setActivationPolicy:"), NSApplicationActivationPolicyRegular);

    // [app activateIgnoringOtherApps:YES];
    msg(app, sel("activateIgnoringOtherApps:"), true);
}

#endif