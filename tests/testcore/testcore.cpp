/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Testing of the core functionality
 */

#include <QtTest/QTest>
#include "core/damper.h"
#include "core/rodsystem.h"
#include "core/databasecables.h"

using namespace RSE::Core;

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void computeDamper();
    void computeRodSystem();
    void cleanupTestCase();

private:
    QString const mkRootPath = "../../../../";
    QString const mkDataPath = mkRootPath + "data/";
    DataBaseCables* mpDataBaseCables;
    Damper* mpDamper;
    RodSystem* mpRodSystem;
};

//! Compare the two floating-point numbers
inline bool fuzzyCompare(double p1, double p2, double eps)
{
    return (qAbs(p1 - p2) <= eps * qMin(qAbs(p1), qAbs(p2)));
}

//! Initialize objects
void TestCore::initTestCase()
{
    mpDataBaseCables = new DataBaseCables(mkDataPath, tr("Провода.txt"));
    mpDamper = new Damper(62, 124, 0.5, 0.4);
    mpRodSystem = new RodSystem({24, 24, 24, 24}, mpDataBaseCables->getItem("АС 120/19"), 3000);
}

//! Determine parameters of a damper
void TestCore::computeDamper()
{
    mpDamper->computeSpring();
    QCOMPARE(mpDamper->springLength(), 0.6);
    QCOMPARE(qFloor(mpDamper->springStiffness()), 6080);
}

//! Compute length of rods
void TestCore::computeRodSystem()
{
    double eps = 1e-3;
    Spans spans = mpRodSystem->computeSpans();
    QVERIFY(fuzzyCompare(spans.u0[0], -0.01847, eps));
    QVERIFY(fuzzyCompare(spans.uL[0], 0.01847, eps));
    QVERIFY(fuzzyCompare(spans.L[0], 23.99497, eps));
}

//! Destroy all the data used
void TestCore::cleanupTestCase()
{
    delete mpDataBaseCables;
    delete mpDamper;
    delete mpRodSystem;
}

QTEST_APPLESS_MAIN(TestCore)

#include "testcore.moc"
