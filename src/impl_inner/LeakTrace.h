#pragma once

/**
 * Allocation tracing, this will cause us to enable the overloaded new operators
 * (Incidentally, sometimes this may also be fairly memory intensive, so we hide
 * it behind an ifdef)
 */
#ifdef JAVAPP_TRACE_ALLOCS

namespace impl_internal {
    void printLeaks();
}

#endif
