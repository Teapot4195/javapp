#include "LeakTrace.h"

#ifdef JAVAPP_TRACE_ALLOCS
#include <cstdio>
#include <cstring>
#include <mutex>
#include <unordered_map>

#include <execinfo.h>
#include <cxxabi.h>

#include <AllocTrace.h>

void** buffer = nullptr;

void impl_internal::printLeaks() {
    allocMapMutex.lock();
    if (allocMap.empty()) {
        fprintf(stderr, "LEAK DETECTION: No leaks are detected.\n");
        return;
    }

    fprintf(stderr, "LEAK DETECTION: Detected leaks are as follows:\n");

    size_t total_leaked = 0;

    for (auto& [key, data] : allocMap) {
        fprintf(stderr, "LEAK DETECTION: - %p (leaked %lu bytes with alignment %d)\n", key, data.size, static_cast<int>(data.align));
#ifdef JAVAPP_TRACE_ALLOCS_STACKTRACE
        fprintf(stderr, "LEAK DETECTION:     Allocation trace is as follows (most recent call first):\n");
        char** backtrace = backtrace_symbols(data.backtraceBuffer, data.backtraceBufferSize);
        for (int i = 0; i < data.backtraceBufferSize; i++) {
            if (data.backtraceBuffer[i] == nullptr)
                continue;
            char* demangled = nullptr;
            if (char *left_par = nullptr, *plus = nullptr;
                ((left_par = strchr(backtrace[i], '('))) && ((plus = strchr(left_par, '+')))) {
                char tmp = '\0';
                std::swap(tmp, *plus);
                int status = 0;
                demangled = abi::__cxa_demangle(left_par + 1, nullptr, nullptr, &status);
                std::swap(tmp, *plus);

                if (demangled) {
                    size_t need_extra = strlen(plus), demangled_size = strlen(demangled);
                    char* res = static_cast<char *>(malloc(demangled_size + need_extra + 1));
                    memcpy(res, demangled, demangled_size);
                    memcpy(res + demangled_size, plus, need_extra);
                    res[demangled_size + need_extra] = '\0';
                    free(demangled);

                    demangled = res;
                }
            }
            fprintf(stderr, "LEAK DETECTION:       [%d] %p: %s\n", i, data.backtraceBuffer[i], demangled ? demangled : backtrace[i]);
            free(demangled);
        }
        free(backtrace);
#endif
        total_leaked += data.size;
    }

#ifndef JAVAPP_TRACE_ALLOCS_STACKTRACE
    fprintf(stderr, "LEAK DETECTION: Build the project with JAVAPP_TRACE_ALLOCS_STACKTRACE for more information.\n");
#endif
    fprintf(stderr, "LEAK DETECTION: A total of %lu bytes still in use at exit.\n", total_leaked);

    allocMapMutex.unlock();
}

bool init_leakprint = [] {
    atexit(impl_internal::printLeaks);
    return true;
}();
#endif
