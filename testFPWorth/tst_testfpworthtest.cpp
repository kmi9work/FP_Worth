#include <QString>
#include <QtTest>
#include <D:\Work\Fires\FP_worth\FP_worth\fptree.h>

class TestFPWorthTest : public QObject
{
    Q_OBJECT

public:
    TestFPWorthTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

TestFPWorthTest::TestFPWorthTest()
{
}

void TestFPWorthTest::initTestCase()
{
}

void TestFPWorthTest::cleanupTestCase()
{
}

void TestFPWorthTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestFPWorthTest)

#include "tst_testfpworthtest.moc"
