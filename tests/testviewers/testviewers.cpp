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
#include "viewers/estimationgraphdata.h"

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
    PointerGraph mpGraph;
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
    mpGraph = std::make_shared<Graph>(tr("Test"));
    mpGraph->setData(new SpaceTimeGraphData(SpaceTimeGraphData::stAccumulatedNaturalLength),
                     new SpaceTimeGraphData(SpaceTimeGraphData::stTime),
                     new KinematicsGraphData(KinematicsGraphData::kDisplacement, AbstractGraphData::dFirst));
    QCOMPARE(3, mpGraph->indicesUniqueData().size());
}

//! Slice graph data
void TestViewers::testDataSlicer()
{
    KLP::PointerResult pResult = std::make_shared<KLP::Result>(mkTestDataPath + "dynamic-impulse-2.klp");
    QCOMPARE(true, mpGraph->createDataSlicer(GraphDataSlicer::sdX, pResult));
    auto& dataSlicer = mpGraph->dataSlicer();
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
    mpKLPGraphViewer->setStandardGraphs();
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
