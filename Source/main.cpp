#include <string>
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

int uBitSerialSend(char *x) {
  return uBit.serial.send(x);
}

char *uBitSerialRead(int x) {
  return uBit.serial.read(x).toCharArray();
}

void uBitMessageBusListen(int item, int type, void (*callback)(void *, void *)) {
  uBit.messageBus.listen(item, type, callback);
}

void *ffiResolver(void *handle, const char *name) {
  if (strcmp(name, "displayScroll") == 0) {
    return (void *)uBitDisplayScroll;
  }
  if (strcmp(name, "sleep") == 0) {
    return (void *)uBitSleep;
  }
  if (strcmp(name, "serialSend") == 0) {
    return (void *)uBitSerialSend;
  }
  if (strcmp(name, "serialRead") == 0) {
    return (void *)uBitSerialRead;
  }
  if (strcmp(name, "messageBusListen") == 0) {
    return (void *)uBitMessageBusListen;
  }
  return NULL;
}

std::string initJS = R"~~~~(let uBit = {
  sleep: ffi('void sleep(int)'),
  display: {
    scroll: ffi('void displayScroll(char *)')
  },
  serial: {
    send: ffi('int serialSend(char *)'),
    read: ffi('char *serialRead(int)')
  },
  messageBus: {
    listen: ffi('void messageBusListen(int, int, void (*)(void *, userdata), userdata)')
  }
};
load = undefined;
print = undefined;
ffi = undefined;
ffi_cb_free = undefined;
getMJS = undefined;
)~~~~";

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();

  struct mjs *mjs = mjs_create();
  mjs_set_ffi_resolver(mjs, ffiResolver);
  mjs_err_t err = mjs_exec(mjs, strcat((char *)initJS.c_str(), jsSource.c_str()), NULL);
  if (err) {
    const char *errStr = mjs_strerror(mjs, err);
    uBit.serial.send(errStr);
  }

  // If main exits, there may still be other fibers running or registered event handlers etc.
  // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
  // sit in the idle task forever, in a power efficient sleep.
  release_fiber();
}
