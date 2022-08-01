/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of aliases used in viewers
 */

#ifndef ALIASVIEWERS_H
#define ALIASVIEWERS_H

#include <QtGlobal>
#include <QVector>
#include <map>

namespace RSE::Viewers
{

class Graph;
using GraphIDType = qint64;
using GraphValueType = double;
using GraphDataset = QVector<GraphValueType>;
using PointerGraph = std::shared_ptr<Graph>;
using MapGraphs = std::map<GraphIDType, PointerGraph>;

}

#endif // ALIASVIEWERS_H
