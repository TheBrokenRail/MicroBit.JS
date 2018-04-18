#include <string>
#include <vector>
#include <cstring>
#include "MicroBit.h"
#include "include/mjs.h"
#include "JSSource.h"

MicroBit uBit;

void uBitDisplayScroll(char *x) {
  uBit.display.scroll(x);
}

void uBitDisplayPrint(char *x) {
  uBit.display.print(x);
}

void uBitDisplayImageSetPixelValue(int x, int y, int alpha) {
  uBit.display.image.setPixelValue(x, y, alpha);
}

void uBitSleep(int x) {
  uBit.sleep(x);
}

int uBitSerialSend(char *x) {
  return uBit.serial.send(x);
}

char *uBitSerialRead(int x) {
  return (char *)uBit.serial.read(x).toCharArray();
}

class uBitListener {
  public:
    uBitListener(int sourceObj, int valueObj, void (*callbackObj)(void *), void *userDataObj) {
      source = sourceObj;
      value = valueObj;
      callback = callbackObj;
      userData = userDataObj;
    };
    int source;
    int value;
    void (*callback)(void *);
    void *userData;
};

std::vector<uBitListener> listeners;

void callListener(MicroBitEvent event) {
  for (unsigned int i = 0; i < listeners.size(); i++) {
    if (event.source == listeners[i].source && event.value == listeners[i].value) {
      listeners[i].callback(listeners[i].userData);
    }
  }
}

void uBitMessageBusListen(int source, int value, void (*callback)(void *), void *userData) {
  uBitListener listener = uBitListener(source, value, callback, userData);
  listeners.push_back(listener);
  uBit.messageBus.listen(source, value, callListener);
}

void *ffiResolver(void *handle, const char *name) {
  if (strcmp(name, "displayScroll") == 0) {
    return (void *)uBitDisplayScroll;
  }
  if (strcmp(name, "displayPrint") == 0) {
    return (void *)uBitDisplayPrint;
  }
  if (strcmp(name, "displayImageSetPixelValue") == 0) {
    return (void *)uBitDisplayImageSetPixelValue;
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
    scroll: ffi('void displayScroll(char*)'),
    print: ffi('void displayPrint(char*)'),
    image: {
      setPixelValue: ffi('void displayImagePixelValue(int, int, int)')
    }
  },
  serial: {
    send: ffi('int serialSend(char*)'),
    read: ffi('char* serialRead(int)')
  },
  messageBus: {
    listen: ffi('void messageBusListen(int, int, void (*)(userdata), userdata)')
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
