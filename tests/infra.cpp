#include "infra.h"

#include <filesystem>
#include <iostream>
#include <stacktrace>

namespace infra {
    // from dawn-winery/dwhbll exceptions/rt_exception_base
    std::string get_prettyprint_trace(const std::stacktrace &trace) {
        std::stringstream stream;
        stream << "Traceback (most recent call first):" << "\n";
        for(auto& entry : trace) {
            const auto function = entry.description().substr(0, entry.description().find("("));

            std::string sourcePosition;
            if (entry.source_file().size() > 0) {
                const auto sourcePath = std::filesystem::path(entry.source_file());
                const auto relativePath = sourcePath.lexically_relative(std::filesystem::current_path());
                const auto filename = relativePath.string().starts_with("../..") ? sourcePath : relativePath;
                sourcePosition = std::format(
                        "{} at {}:{}",
                        function.data(), filename.c_str(), entry.source_line());
            } else if (!function.empty()) {
                sourcePosition = function;
            } else if (!entry.description().empty()) {
                sourcePosition = entry.description();
            } else {
                sourcePosition = "???";
            }

            const auto info = std::format(
                    "[{:#018x}] {}\n",
                    entry.native_handle(), sourcePosition.data());
            stream << (info);
        }

        return stream.str();
    }

    void dump_stacktrace() {
        std::stacktrace trace = std::stacktrace::current();
        auto result = get_prettyprint_trace(trace);

        std::cerr << result << std::endl;
    }

    void fail() {
        ++failed;
        dump_stacktrace();
    }

    void fail(const std::string &msg) {
        std::cout << msg << std::endl;
        fail();
    }

    void unexpected(std::exception &e) {
        ++failed;

        // TODO: e->printStackTrace(); in the future
        dump_stacktrace();
    }

    void equal(shared<Object> x, shared<Object> y) {
        if (x == null ? y == null : x->equals(y))
            pass();
        else {
            std::cout << x->toString() << " not equal to " << y->toString() << std::endl;
            fail();
        }
    }
}
