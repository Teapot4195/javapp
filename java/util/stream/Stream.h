#pragma once

#include "BaseStream.h"
#include <Object.h>

#include "java/util/function/Consumer.h"

namespace java::util::stream {
    class Stream : public virtual Object, public virtual BaseStream {
    public:
        class Builder;

        ~Stream() override = default;
    };

    class Stream::Builder : public virtual Object, public virtual function::Consumer {
    public:
        ~Builder() override = default;

        void accept(shared<Object> T) override = 0;

        virtual shared<Builder> add(shared<Object> T);

        virtual shared<Stream> build() = 0;
    };
}
