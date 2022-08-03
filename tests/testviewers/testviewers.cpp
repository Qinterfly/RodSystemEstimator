/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Testing the widgets which are used to view distributions of parameters
 */

#include <QtTest>
#include <QtWidgets>
#include "core/numericalutilities.h"
#include "klp/result.h"
#include "viewers/apputilities.h"
#include "viewers/convergenceviewer.h"
#include "viewers/klpgraphviewer.h"
#include "viewers/graph.h"
#include "viewers/spacetimegraphdata.h"
#include "viewers/kinematicsgraphdata.h"
#include "viewers/energygraphdata.h"

using namespace RSE::Core;
using namespace RSE::Viewers;
using namespace RSE::Utilities::Numerical;

class TestViewers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConvergenceViewer();
    void testGraphs();
    void testDataSlicer();
    void testKLPGraphViewer();
    void cleanupTestCase();

private:
    QString const mkRootPath = "../../../../";
    QString const mkDataPath = mkRootPath + "data/";
    QString const mkOutputPath = mkDataPath + "Output/";
    QString const mkTestDataPath = mkRootPath + "tests/data/";
    QSettings* mpSettings;
    ConvergenceViewer* mpConvergenceViewer;
    KLPGraphViewer* mpKLPGraphViewer;
    MapGraphs mGraphs;
};

//! Initialize viewers
void TestViewers::initTestCase()
{
    mpSettings = new QSettings("Settings.ini", QSettings::IniFormat);
    RSE::Utilities::App::setStyle();
}

//! Represent convergence of viscosities
void TestViewers::testConvergenceViewer()
{
    QString pathFile = mkOutputPath + tr("optimal.txt");
    mpConvergenceViewer = new ConvergenceViewer(pathFile);
    RSE::Utilities::App::moveToCenter(mpConvergenceViewer);
    mpConvergenceViewer->plot();
}

//! Create graphs of different types
void TestViewers::testGraphs()
{
    const int kNumGraphs = 3;
    // Create multiple graphs
    for (int i = 0; i != kNumGraphs; ++i)
    {
        auto pGraph = std::make_shared<Graph>(tr("Тест %1").arg(i));
        mGraphs.insert({i, pGraph});
    }
    // [Xi, t, Ut]
    mGraphs[0]->setData(new SpaceTimeGraphData(SpaceTimeGraphData::stParameter),
                        new SpaceTimeGraphData(SpaceTimeGraphData::stTime),
                        new KinematicsGraphData(KinematicsGraphData::kSpeed));
    mGraphs[0]->setAxesLabels({tr("Параметр"), tr("Время"), tr("Скорость")});
    // [SS, t, U]
    mGraphs[1]->setData(new SpaceTimeGraphData(SpaceTimeGraphData::stAccumulatedNaturalLength),
                        new SpaceTimeGraphData(SpaceTimeGraphData::stTime),
                        new KinematicsGraphData(KinematicsGraphData::kDisplacement, AbstractGraphData::dFirst));
    mGraphs[1]->setAxesLabels({tr("Накопленная длина"), tr("Время"), tr("Перемещение")});
    // [t, Ef]
    mGraphs[2]->setData(new SpaceTimeGraphData(SpaceTimeGraphData::stTime),
                        new EnergyGraphData(EnergyGraphData::eFull));
    mGraphs[2]->setAxesLabels({tr("Время"), tr("Полная энергия"), tr("")});
}

//! Slice graph data
void TestViewers::testDataSlicer()
{
    KLP::PointerResult pResult = std::make_shared<KLP::Result>(mkTestDataPath + "dynamic-impulse-2.klp");
    PointerGraph pGraph = mGraphs[1];
    QCOMPARE(true, pGraph->createDataSlicer(GraphDataSlicer::sdX, pResult));
    auto& dataSlicer = pGraph->dataSlicer();
    QVERIFY(fuzzyCompare(dataSlicer.setClosestValue(0.5), 0.52, 1e-3));
}

//! Represent content of the KLP file
void TestViewers::testKLPGraphViewer()
{
    mpKLPGraphViewer = new KLPGraphViewer(mkTestDataPath, *mpSettings);
    RSE::Utilities::App::moveToCenter(mpKLPGraphViewer);
    QStringList pathFiles = {"modal.klp", "dynamic.klp", "dynamic-impulse-1.klp", "dynamic-impulse-2.klp"};
    for (QString& path : pathFiles)
        path = mkTestDataPath + path;
    mpKLPGraphViewer->openResults(pathFiles);
    mpKLPGraphViewer->setGraphs(std::move(mGraphs));
    mpKLPGraphViewer->show();
}

//! Destroy all the viewers used
void TestViewers::cleanupTestCase()
{
    qApp->exec();
    delete mpConvergenceViewer;
    delete mpKLPGraphViewer;
    delete mpSettings;
}

QTEST_MAIN(TestViewers)

#include "testviewers.moc"
