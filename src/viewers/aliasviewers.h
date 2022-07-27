/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
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
using GraphDataset = QVector<float>;
using MapGraphs = std::map<GraphIDType, std::shared_ptr<Graph>>;

}

#endif // ALIASVIEWERS_H
