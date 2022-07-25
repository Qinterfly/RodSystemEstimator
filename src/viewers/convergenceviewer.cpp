/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the ConvergenceViewer class
 */

#include <QVBoxLayout>
#include "convergenceviewer.h"

using namespace RSE::Viewers;

ConvergenceViewer::ConvergenceViewer(QString const& pathFile, QWidget* pParent)
    : QWidget(pParent), mkPathFile(pathFile)
{
    initialize();
    plot();
}

ConvergenceViewer::~ConvergenceViewer()
{
    delete mpFigure;
}

//! Initialize the widget
void ConvergenceViewer::initialize()
{
    // Geometry
    setGeometry(0, 0, 600, 600);
    // Configure QCustomPlot
    mpFigure = new QCustomPlot();
    mpFigure->setAntialiasedElement(QCP::AntialiasedElement::aeAll, true);
    mpFigure->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // Specify options of axes
    mpFigure->xAxis->setLabel(tr("Число расчетных форм колебаний"));
    mpFigure->yAxis->setLabel(tr("Вязкость демпфера, Н/c"));
    mpFigure->xAxis->setNumberFormat("g");
    mpFigure->yAxis->setNumberFormat("gb");
    // Configure the legend
    mpFigure->legend->setVisible(true);
    // Create a set of available colors for graphs
    mAvailableColors = {"red", "green", "blue", "black", "orange", "cyan", "magenta", "gray", "purple", "darkyellow"};
    // Construct a set of marker styles
    mAvailableShapes << QCPScatterStyle::ssDisc << QCPScatterStyle::ssPlus << QCPScatterStyle::ssCircle << QCPScatterStyle::ssCross
                     << QCPScatterStyle::ssSquare <<  QCPScatterStyle::ssDiamond << QCPScatterStyle::ssStar << QCPScatterStyle::ssTriangle
                     << QCPScatterStyle::ssTriangleInverted << QCPScatterStyle::ssCrossSquare;
    // Arrangement of widgets
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->addWidget(mpFigure);
    setLayout(pLayout);
}

//! Represent the convergence
void ConvergenceViewer::plot()
{
    const int kShapeSize = 5;
    // Preapare the data to be plotted
    if (!read())
        return;
    int numDampers = mDampingValues.rows();
    int numCalcModes = mCalcModes.size();
    QVector<double> xValues;
    xValues.reserve(mCalcModes.size());
    std::copy(mCalcModes.cbegin(), mCalcModes.cend(), std::back_inserter(xValues));
    QVector<double> yValues(numCalcModes);
    // Plot the graphs
    int iColor = 0, iShape = 0;
    int numAvailableColors = mAvailableColors.size();
    int numAvailableShapes = mAvailableShapes.size();
    mpFigure->clearGraphs();
    for (int iDamper = 0; iDamper != numDampers; ++iDamper)
    {
        // Create the graph
        for (int jMode = 0; jMode != numCalcModes; ++jMode)
            yValues[jMode] = mDampingValues[iDamper][jMode];
        QCPGraph* pGraph = mpFigure->addGraph();
        pGraph->setData(xValues, yValues);
        pGraph->setName(tr("Демпфер: №%1").arg(QString::number(iDamper + 1)));
        // Assign the coor
        if (iColor >= numAvailableColors - 1)
            iColor = 0;
        pGraph->setPen(QPen(mAvailableColors[iColor]));
        ++iColor;
        // Set the marker
        if (iShape >= numAvailableShapes - 1)
            iShape = 0;
        pGraph->setScatterStyle(QCPScatterStyle(mAvailableShapes[iShape], kShapeSize));
        ++iShape;
    }
    // Rescale axes and update
    mpFigure->rescaleAxes();
    mpFigure->xAxis->setRangeLower(mCalcModes[0]);
    mpFigure->replot();
}

//! Read the file contained viscosities of dampers
bool ConvergenceViewer::read()
{
    QFile file(mkPathFile);
    if (!file.open(QIODeviceBase::ReadOnly))
        return false;
    QTextStream fileStream(&file);
    QString currentLine;
    // Erase the previous set of values
    mCalcModes.clear();
    mDampingValues.clear();
    // Number of computational modes
    int iValue;
    currentLine = fileStream.readLine();
    QTextStream stringStream(&currentLine);
    while (!stringStream.atEnd())
    {
        stringStream >> iValue;
        mCalcModes.push_back(iValue);
        stringStream.skipWhiteSpace();
    }
    int numCalcModes = mCalcModes.size();
    // Count the number of dampers
    quint64 startPosDampers = fileStream.pos();
    int numDampers = 0;
    while (fileStream.readLineInto(&currentLine))
        ++numDampers;
    // Viscosities of dampers
    mDampingValues.resize(numDampers, numCalcModes);
    fileStream.seek(startPosDampers);
    double value;
    for (int i = 0; i != numDampers; ++i)
    {
        currentLine = fileStream.readLine();
        QTextStream currentStream(&currentLine);
        for (int j = 0; j != numCalcModes; ++j)
        {
            currentStream >> value;
            mDampingValues[i][j] = value;
        }
    }
    return true;
}
