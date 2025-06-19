#include "teststatsmedian/tst_teststatsmedian.h"
#include "testgeneexpressionmerger/tst_testgeneexprhelper.h"

int main(int argc, char** argv)
{
    TestStatsMedian tsm;
    // QTest::qExec(&tsm, argc, argv);

    TestGeneExprHelper tgeh;
    QTest::qExec(&tgeh, argc, argv);
    return 0;
}
