#include "Consumer.h"

namespace java::util::function {
    void Consumer::accept(shared<Object> T) {
        inner(std::move(T));
    }

    shared<Consumer> Consumer::andThen(const shared<Consumer> after) {
        return alloc<Consumer>(lambda(const shared<Object> &obj) {
            self->accept(obj);
            after->accept(obj);
        });
    }
}
