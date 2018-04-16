#include <cstring>
#include "MicroBit.h"
#include "include/mjs.h"
#include "JSSource.h"

MicroBit uBit;

void displayScroll(char *x) {
  uBit.display.scroll(x);
}

void *ffiResolver(void *handle, const char *name) {
  if (strcmp(name, "displayScroll") == 0) {
    return (void *)displayScroll;
  }
  return NULL;
}

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();

    struct mjs *mjs = mjs_create();
    mjs_set_ffi_resolver(mjs, ffiResolver);
    mjs_exec(mjs, strcat("let micro = {}; micro.displayScroll = ffi('void displayScroll(char *)'); ", (const char)Source_main_js), NULL);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
