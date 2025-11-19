#include "Stream.h"

namespace java::util::stream {
    shared<Stream::Builder> Stream::Builder::add(const shared<Object> T) {
        accept(T);
        return from_self<Builder>();
    }
}
