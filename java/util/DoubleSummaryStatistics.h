#pragma once

#include <Object.h>

#include <java/util/function/DoubleConsumer.h>

namespace java::util {
    class DoubleSummaryStatistics : public virtual Object, public virtual function::DoubleConsumer {
        double count;
        double sum;
        double min;
        double max;

    public:
        ~DoubleSummaryStatistics() override = default;

        DoubleSummaryStatistics();

        DoubleSummaryStatistics(long long count, double min, double max, double sum);

        void accept(double value) override;

        virtual void combine(shared<DoubleSummaryStatistics> other);

        double getAverage() const;

        long long getCount() const;

        double getMax() const;

        double getMin() const;

        double getSum() const;

        shared<String> toString() override;
    };

}
