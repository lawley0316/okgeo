#ifndef TST_TESTGENEEXPRHELPER_H
#define TST_TESTGENEEXPRHELPER_H

#include <QtTest>

class TestGeneExprHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestGeneExprHelper(QObject *parent = nullptr);


private slots:
    void test_Aggregate_Mean();
    void test_Aggregate_Median();
    void test_Aggregate_Max();
    void test_Aggregate_Min();
};

#endif // TST_TESTGENEEXPRHELPER_H
