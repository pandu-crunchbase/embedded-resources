#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "circular_buffer.h"

// The definition of our circular buffer structure is hidden from the user
struct circular_buf_t {
    uint8_t * buffer;
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
};

#pragma mark - Private Functions -

static void advance_pointer(cbuf_handle_t cbuf) {
    assert(cbuf);
    if(circular_buf_full(cbuf)) {
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    }
    cbuf->head = (cbuf->head + 1) % cbuf->max;
    // We mark full because we will advance tail on the next time around
    cbuf->full = (cbuf->head == cbuf->tail);
}

void cbuf_put(cbuf_handle_t cbuf, uint8_t data) {
    assert(cbuf && cbuf->buffer);
    cbuf->buffer[cbuf->head] = data;
    advance_pointer(cbuf);
}

int cbuf_get(cbuf_handle_t cbuf, uint8_t * data) {
    assert(cbuf && data && cbuf->buffer);
    int r = -1;
    if(!cbuf_empty(cbuf)) {
        *data = cbuf->buffer[cbuf->tail];
        retreat_pointer(cbuf);
        r = 0;
    }
    return r;
}

bool cbuf_empty(cbuf_handle_t cbuf) {
    assert(cbuf);
    return (!circular_buf_full(cbuf) && (cbuf->head == cbuf->tail));
}

bool circular_buf_full(cbuf_handle_t cbuf)
{
    assert(cbuf);

    return cbuf->full;
}

