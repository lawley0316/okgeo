#include "src/utils/stats.h"
#include "tst_teststatsmedian.h"

#include <QString>

TestStatsMedian::TestStatsMedian() {}

TestStatsMedian::~TestStatsMedian() {}

void TestStatsMedian::test_empty() {
    try {
        std::vector<int> v;
        stats::MedianInplace(v.begin(), v.end());
    } catch (const std::invalid_argument& exc) {
        QCOMPARE(QString(exc.what()), QString("no median for empty data"));
    } catch (...) {
        QFAIL("unexpected exception thrown");
    }
}

void TestStatsMedian::test_single() {
    std::vector<int> v = {68};
    double m = stats::MedianInplace(v.begin(), v.end());
    QCOMPARE(m, 68.0);
}

void TestStatsMedian::test_double() {
    std::vector<int> v = {68, 90};
    double m = stats::MedianInplace(v.begin(), v.end());
    QCOMPARE(m, 79.0);
}

void TestStatsMedian::test_odd() {
    // ordered: {6, 18, 20, 49, 64, 68, 76, 78, 90}
    std::vector<int> v = {68, 90, 78, 6, 49, 64, 20, 76, 18};
    double m = stats::MedianInplace(v.begin(), v.end());
    QCOMPARE(m, 64.0);
}

void TestStatsMedian::test_even() {
    // ordered: {6, 18, 20, 49, 64, 68, 76, 78, 90, 96}
    std::vector<int> v = {68, 90, 78, 6, 49, 96, 64, 20, 76, 18};
    double m = stats::MedianInplace(v.begin(), v.end());
    QCOMPARE(m, 66.0);
}

void TestStatsMedian::test_same() {
    std::vector<int> v = {68, 68, 68, 68, 68};
    double m = stats::MedianInplace(v.begin(), v.end());
    QCOMPARE(m, 68.0);
}

void TestStatsMedian::test_negative()
{
    std::vector<int> v = {-5, -1, -3, -2, -4};
    QCOMPARE(stats::MedianInplace(v.begin(), v.end()), -3.0);
}

void TestStatsMedian::test_float()
{
    // ordered: {6.3, 18.9, 20.7, 49.4, 64.6, 68.0, 76.8, 78.2, 90.1, 96.5}
    std::vector<double> v = {68.0, 90.1, 78.2, 6.3, 49.4, 96.5, 64.6, 20.7, 76.8, 18.9};
    double m = stats::MedianInplace(v.begin(), v.end());
    QCOMPARE(m, 66.3);
}
