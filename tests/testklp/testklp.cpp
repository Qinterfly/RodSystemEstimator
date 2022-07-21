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
    void cleanupTestCase();

private:
    QString const mkRootPath = "../../../../";
    QString const mkDataPath = mkRootPath + "tests/data/";
    Result* mpModalResult = nullptr;
};

//! Read the output file which consists of modal data
void TestKLP::readModal()
{
    mpModalResult = new Result(mkDataPath + "modal.klp");
    qint64 iFrame = 0;
    auto collection = mpModalResult->getFrameCollection(iFrame);
    QCOMPARE(collection.naturalLength.size(), 2388);
}

//! Destroy all the data used
void TestKLP::cleanupTestCase()
{
    delete mpModalResult;
}

QTEST_APPLESS_MAIN(TestKLP)

#include "testklp.moc"
