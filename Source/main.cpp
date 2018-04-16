#include <cstring>
#include "MicroBit.h"
#include "include/mjs.h"
#include "JSSource.h"

MicroBit uBit;

void uBitDisplayScroll(char *x) {
  uBit.display.scroll(x);
}

void uBitSleep(int x) {
  uBit.sleep(x);
}

void uBitSerialSend(char *x) {
  uBit.serial.send(x);
}

void uBitSerialRead(int x) {
  uBit.serial.read(x);
}

void *ffiResolver(void *handle, const char *name) {
  if (strcmp(name, "displayScroll") == 0) {
    return (void *)uBitDisplayScroll;
  }
  if (strcmp(name, "sleep") == 0) {
    return (void *)uBitSleep;
  }
  if (strcmp(name, "serialWrite") == 0) {
    return (void *)uBitSerialWrite;
  }
  if (strcmp(name, "serialRead") == 0) {
    return (void *)uBitSerialRead;
  }
  return NULL;
}

const char *initJS = R"~~~~(let uBit = {};
uBit.sleep = ffi('void sleep(int)');
uBit.display = {};
uBit.display.scroll = ffi('void displayScroll(char *)');
uBit.serial = {};
uBit.serial.write = ffi('void serialWrite(char *)');
uBit.serial.read = ffi('void serialRead(int)');
)~~~~";

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();

    struct mjs *mjs = mjs_create();
    mjs_set_ffi_resolver(mjs, ffiResolver);
    mjs_err_t err = mjs_exec(mjs, strcat(initJS, jsSource), NULL);
    if (err) {
      const char *errStr = mjs_strerror(mjs, err);
      uBit.serial.write(errStr);
    }

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
