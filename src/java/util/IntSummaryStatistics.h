#pragma once

#include <Integer.h>
#include <java/util/function/IntConsumer.h>
#include <Object.h>

namespace java::util {
    class IntSummaryStatistics : public virtual Object, public virtual function::IntConsumer {
        long long count, sum;
        int min = Integer::MAX_VALUE, max = Integer::MIN_VALUE;

    public:
        ~IntSummaryStatistics() override = default;

        IntSummaryStatistics() = default;

        IntSummaryStatistics(long long count, int min, int max, long long sum);

        void accept(int value) override;

        virtual void combine(shared<IntSummaryStatistics> other);

        double getAverage() const;

        long long getCount() const;

        int getMax() const;

        int getMin() const;

        long long getSum() const;

        shared<String> toString() override;
    };
}
