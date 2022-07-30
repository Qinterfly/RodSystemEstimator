/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Testing the widgets which are used to view distributions of parameters
 */

#include <QtTest>
#include <QtWidgets>
#include "klp/result.h"
#include "viewers/apputilities.h"
#include "viewers/convergenceviewer.h"
#include "viewers/klpgraphviewer.h"
#include "viewers/graph.h"
#include "viewers/spacetimegraphdata.h"
#include "viewers/kinematicsgraphdata.h"

using namespace RSE::Core;
using namespace RSE::Viewers;

class TestViewers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConvergenceViewer();
    void testGraphs();
    void testGraphData();
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
    const int kNumGraphs = 5;
    // Create multiple graphs
    for (int i = 0; i != kNumGraphs; ++i)
    {
        auto pGraph = std::make_shared<Graph>(tr("Тест %1").arg(i));
        mGraphs.insert({i, pGraph});
    }
    // Create graph data of several types
    AbstractGraphData* pCoordinate = new SpaceTimeGraphData(SpaceTimeGraphData::stCoordiante);
    AbstractGraphData* pParameter = new SpaceTimeGraphData(SpaceTimeGraphData::stParameter);
    AbstractGraphData* pLength = new SpaceTimeGraphData(SpaceTimeGraphData::stAccumulatedNaturalLength);
    AbstractGraphData* pTime = new SpaceTimeGraphData(SpaceTimeGraphData::stTime);
    AbstractGraphData* pDisplacement = new KinematicsGraphData(KinematicsGraphData::kDisplacement);
    // Assign graph data
    mGraphs[0]->setData(pCoordinate, pParameter);
    mGraphs[1]->setData(pLength, pTime, pDisplacement);
}

//! Retrieve data of graphs
void TestViewers::testGraphData()
{
    KLP::Result result(mkTestDataPath + "dynamic-impulse-1.klp");
    auto collection = result.getFrameCollection(1);
    auto pXData = mGraphs[0]->data()[1];
    auto limits = pXData->getDatasetLimits(collection);
    float foundValue = pXData->findClosestValue(collection, 2.0);
    QCOMPARE(104, limits.indices.second);
    QCOMPARE(3.0, limits.values.second);
    QCOMPARE(2.0, foundValue);
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
