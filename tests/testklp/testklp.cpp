/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Testing of the KLP functionality
 */

#include <QtTest/QTest>
#include "klp/result.h"

using namespace KLP;

class TestKLP : public QObject
{
    Q_OBJECT

private slots:
    void readModal();
    void readDynamic();
    void cleanupTestCase();

private:
    QString const mkRootPath = "../../../../";
    QString const mkDataPath = mkRootPath + "tests/data/";
    Result* mpModalResult = nullptr;
    Result* mpDynamicResult = nullptr;
};

//! Read the output file which consists of modal data
void TestKLP::readModal()
{
    mpModalResult = new Result(mkDataPath + "modal.klp");
    auto collection = mpModalResult->getFrameCollection(0);
    QCOMPARE(mpModalResult->info().numTimeRecords, 1);
    QCOMPARE(collection.naturalLength.size(), 2388);
    QCOMPARE(collection.numRods, 28);
}

//! Read the output file which consists of dynamic data
void TestKLP::readDynamic()
{
    mpDynamicResult = new Result(mkDataPath + "dynamic.klp");
    auto collection = mpModalResult->getFrameCollection(1);
    QCOMPARE(mpDynamicResult->info().numTimeRecords, 660);
    QCOMPARE(collection.numRods, 28);
}

//! Destroy all the data used
void TestKLP::cleanupTestCase()
{
    delete mpModalResult;
    delete mpDynamicResult;
}

QTEST_APPLESS_MAIN(TestKLP)

#include "testklp.moc"
