#pragma once

#include <cstdint>

constexpr static double load_factor_limit = 0.75f;
constexpr static double tombstone_factor_limit = 0.1f;
constexpr static size_t default_bucket_count = 22;

static_assert(default_bucket_count % 2 == 0);
static_assert(default_bucket_count > 0);