/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Testing the widgets which are used to view distributions of parameters
 */

#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets>
#include <QScreen>
#include "core/utilities.h"
#include "viewers/convergenceviewer.h"

using namespace RSE::Core;
using namespace RSE::Viewers;

class TestViewers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConvergenceViewer();
    void cleanupTestCase();

private:
    QString const mkRootPath = "../../../../";
    QString const mkDataPath = mkRootPath + "data/";
    QString const mkOutputPath = mkDataPath + "Output/";
    QRect mGeometryWidget;
    ConvergenceViewer* mpConvergenceViewer;
};

//! Initialize viewers
void TestViewers::initTestCase()
{
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(RSE::Utilities::File::loadFileContent(":/styles/modern.qss"));
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    uint fontSize = 10;
    qApp->setFont(QFont("Source Sans Pro", fontSize));
    // Position of widgets
    QSize widgetSize = {600, 600};
    QSize screenSize = qApp->primaryScreen()->availableGeometry().size();
    mGeometryWidget.setLeft((screenSize.width() - widgetSize.width()) / 2);
    mGeometryWidget.setTop((screenSize.height() - widgetSize.height()) / 2);
    mGeometryWidget.setSize(widgetSize);
}

//! Represent convergence of viscosities
void TestViewers::testConvergenceViewer()
{
    QString pathFile = mkOutputPath + tr("optimal.txt");
    mpConvergenceViewer = new ConvergenceViewer(pathFile);
    mpConvergenceViewer->plot();
    mpConvergenceViewer->setGeometry(mGeometryWidget);
}

//! Destroy all the viewers used
void TestViewers::cleanupTestCase()
{
    delete mpConvergenceViewer;
    qApp->exec();
}

QTEST_MAIN(TestViewers)

#include "testviewers.moc"
