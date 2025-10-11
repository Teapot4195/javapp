#include "IntSummaryStatistics.h"

#include <String.h>

namespace java::util {
    IntSummaryStatistics::IntSummaryStatistics(long long count, int min, int max, long long sum)
        : count(count), sum(sum), min(min), max(max) {}

    void IntSummaryStatistics::accept(int value) {
        count++;
        sum += value;
        min = std::min(min, value);
        max = std::max(max, value);
    }

    void IntSummaryStatistics::combine(shared<IntSummaryStatistics> other) {
        count += other->count;
        sum += other->sum;
        min = std::min(min, other->min);
        max = std::max(max, other->max);
    }

    double IntSummaryStatistics::getAverage() const {
        if (count == 0)
            return 0;
        return static_cast<double>(sum) / static_cast<double>(count);
    }

    long long IntSummaryStatistics::getCount() const {
        return count;
    }

    int IntSummaryStatistics::getMax() const {
        return max;
    }

    int IntSummaryStatistics::getMin() const {
        return min;
    }

    long long IntSummaryStatistics::getSum() const {
        return sum;
    }

    shared<String> IntSummaryStatistics::toString() {
        return alloc<String>(std::format("IntSummaryStatistics{{avg:{},count:{},max:{},min:{},sum:{}}}",
            getAverage(), getCount(), getMax(), getMin(), getSum()
        ));
    }
}
