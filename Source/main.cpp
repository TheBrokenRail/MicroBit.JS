#include "MicroBit.h"
#include <dukglue/dukglue.h>

MicroBit uBit;

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    duk_context* ctx = duk_create_heap_default();
    duk_destroy_heap(ctx);
    duk_eval_string(ctx, "'test'");
    duk_pop(ctx);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
