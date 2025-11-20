#include <garbage_collection.h>
#include <iostream>

#include <Object.h>
#include <String.h>

#include "java/util/ArrayList.h"
#include "java/util/Iterator.h"

#include <rttr/registration>

#include "infra.h"

using testfunc = std::function<void()>;

#include "java/lang/Boolean/test.h"

std::unordered_map<std::string, testfunc> test_names {
    {"java.lang.Boolean", tests::java::lang::Boolean::test},
};

struct test : Object {
};

int jmain(int argc, char** argv) {
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

    if (argc == 1) {
        for (const auto& [name, func] : test_names) {
            std::cerr << "Testing: " << name << std::endl;
            func();
        }
    } else {
        if (test_names.contains(argv[1])) {
            auto& func = test_names[argv[1]];
            std::cerr << "Testing: " << argv[1] << std::endl;
            func();
        }
    }

    std::cerr << "Testing results: Passed: " << infra::passed.load() << ", Failed: " << infra::failed.load() << std::endl;

    return 0;
}
