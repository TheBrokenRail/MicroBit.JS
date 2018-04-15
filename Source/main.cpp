#include "MicroBit.h"
#include "include/mjs.h"
#include <cstring>

MicroBit uBit;

void displayScroll(char* x) {
  uBit.display.scroll(x);
}

void *getFFI(void *handle, const char *name) {
  if (strcmp(name, "displayScroll") == 0) return displayScroll;
  return NULL;
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    struct mjs *mjs = mjs_create();
    mjs_set_ffi_resolver(mjs, getFFI);
    mjs_exec(mjs, "let micro = {}; micro.displayScroll = ffi('void displayScroll(char)'); micro.displayScroll('1234')", NULL);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
