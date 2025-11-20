#include "test.h"

#include <Object.h>

#include <Boolean.h>
#include <infra.h>

namespace tests::java::lang::Boolean {
    void test() {
        infra::check(::Boolean::valueOf(true) == ::Boolean::TRUE);
        infra::check(::Boolean::valueOf(false) == ::Boolean::FALSE);

        infra::check(::Boolean::parseBoolean("TRUE"_s));
        infra::check(::Boolean::parseBoolean("true"_s));
        infra::check(::Boolean::parseBoolean("TrUe"_s));

        infra::icheck(::Boolean::parseBoolean("false"_s));
        infra::icheck(::Boolean::parseBoolean("FALSE"_s));
        infra::icheck(::Boolean::parseBoolean("FaLsE"_s));
        infra::icheck(::Boolean::parseBoolean(null));
        infra::icheck(::Boolean::parseBoolean("junk"_s));
        infra::icheck(::Boolean::parseBoolean("truee"_s));
    }
}
