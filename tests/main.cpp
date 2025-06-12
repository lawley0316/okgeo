#include "teststatsmedian/tst_teststatsmedian.h"

int main(int argc, char** argv)
{
    TestStatsMedian tsm;
    QTest::qExec(&tsm, argc, argv);
    return 0;
}
