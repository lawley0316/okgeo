#ifndef TST_TESTSTATSMEDIAN_H
#define TST_TESTSTATSMEDIAN_H

#include <QtTest>


class TestStatsMedian : public QObject
{
    Q_OBJECT

public:
    TestStatsMedian();
    ~TestStatsMedian();

private slots:
    void test_empty();
    void test_single();
    void test_double();
    void test_odd();
    void test_even();
    void test_same();
    void test_negative();
    void test_float();
};

#endif
