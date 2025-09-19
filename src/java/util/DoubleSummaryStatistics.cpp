#include "DoubleSummaryStatistics.h"

#include <../../src/Double.h>
#include <format>
#include <../../src/String.h>

namespace java::util {
    DoubleSummaryStatistics::DoubleSummaryStatistics()
        : DoubleConsumer(), count(0), sum(0), min(Double::POSITIVE_INFINITY), max(Double::NEGATIVE_INFINITY) {}

    DoubleSummaryStatistics::DoubleSummaryStatistics(const long long count, const double min, const double max, const double sum)
        : DoubleConsumer(), count(count), sum(sum), min(min), max(max) {}

    void DoubleSummaryStatistics::accept(const double value) {
        ++count;
        sum += value;
        min = std::min(min, value);
        max = std::max(max, value);
    }

    void DoubleSummaryStatistics::combine(const shared<DoubleSummaryStatistics> other) {
        count += other->count;
        sum += other->sum;
        min = std::min(min, other->min);
        max = std::max(max, other->max);
    }

    double DoubleSummaryStatistics::getAverage() const {
        if (count == 0)
            return 0;
        return sum / count;
    }

    long long DoubleSummaryStatistics::getCount() const {
        return count;
    }

    double DoubleSummaryStatistics::getMax() const {
        return max;
    }

    double DoubleSummaryStatistics::getMin() const {
        return min;
    }

    double DoubleSummaryStatistics::getSum() const {
        return sum;
    }

    shared<String> DoubleSummaryStatistics::toString() {
        return alloc<String>(std::format("DoubleSummaryStatistics{{avg:{},count:{},max:{},min:{},sum:{}}}",
            getAverage(), getCount(), getMax(), getMin(), getSum()
        ));
    }
}
