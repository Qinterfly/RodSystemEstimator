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

private:
    QString const mkRootPath = "../../../../";
    QString const mkDataPath = mkRootPath + "tests/data/";
};

//! Read the output file which consists of modal data
void TestKLP::readModal()
{
    Result(mkDataPath + "modal.klp");
}

QTEST_APPLESS_MAIN(TestKLP)

#include "testklp.moc"
