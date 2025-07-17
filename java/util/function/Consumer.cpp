#include "Consumer.h"

namespace java::util::function {
    void Consumer::accept(std::shared_ptr<Object> T) {
        inner(std::move(T));
    }

    shared<Consumer> Consumer::andThen(shared<Consumer> after) {
        return alloc<Consumer>([&](shared<Object> obj) {
            this->accept(obj);
            after->accept(obj);
        });
    }
}
