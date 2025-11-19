#include <garbage_collection.h>
#include <iostream>

#include <Object.h>
#include <String.h>

#include "java/util/ArrayList.h"
#include "java/util/Iterator.h"

#include <rttr/registration>

struct test : Object {
};

int jmain(int argc, char** argv) {
    Object::registerType<test>();

    std::vector<shared<Object>> t;

    shared<test> m;
    for (int i = 0; i < 1024*1024; i++) {
        t.push_back(alloc<test>());
    }

    std::cout << "now sleeping" << std::endl;

    while (true) {
        javapp::garbage_collection::gc_maybe();
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
        break;
    }

    return 0;
}
