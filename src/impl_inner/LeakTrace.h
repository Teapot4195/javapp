#pragma once

/**
 * Allocation tracing, this will cause us to enable the overloaded new operators
 * (Incidentally, sometimes this may also be fairly memory intensive,
 */
#ifdef JAVAPP_TRACE_ALLOCS

namespace impl_internal {
    void printLeaks();
}

#endif
