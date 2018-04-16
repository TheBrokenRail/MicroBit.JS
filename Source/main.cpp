#include <cstring>
#include "MicroBit.h"
#include "include/mjs.h"
#include "JSSource.h"

MicroBit uBit;

void displayScroll(char *x) {
  uBit.display.scroll(x);
}

void sleep(int x) {
  uBit.sleep(x);
}

void *ffiResolver(void *handle, const char *name) {
  if (strcmp(name, "displayScroll") == 0) {
    return (void *)displayScroll;
  }
  if (strcmp(name, "sleep") == 0) {
    return (void *)sleep;
  }
  return NULL;
}

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();

    struct mjs *mjs = mjs_create();
    mjs_set_ffi_resolver(mjs, ffiResolver);
    char str[] = "let uBit = {}; uBit.sleep = ffi('void sleep(int)'); uBit.display = {}; uBit.display.scroll = ffi('void displayScroll(char *)'); ";
    mjs_exec(mjs, strcat(str, jsSource.c_str()), NULL);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
