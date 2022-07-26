/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Testing the widgets which are used to view distributions of parameters
 */

#include <QtTest>
#include <QtWidgets>
#include "viewers/apputilities.h"
#include "viewers/convergenceviewer.h"
#include "viewers/klpgraphviewer.h"

using namespace RSE::Core;
using namespace RSE::Viewers;

class TestViewers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConvergenceViewer();
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

//! Represent content of the KLP file
void TestViewers::testKLPGraphViewer()
{
    mpKLPGraphViewer = new KLPGraphViewer(mkTestDataPath, *mpSettings);
    RSE::Utilities::App::moveToCenter(mpKLPGraphViewer);
    mpKLPGraphViewer->openResults({mkTestDataPath + "modal.klp", mkTestDataPath + "dynamic.klp"});
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
