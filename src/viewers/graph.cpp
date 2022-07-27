/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Graph class
 */

#include "graph.h"

using namespace RSE::Viewers;

GraphIDType Graph::smMaxGraphID = 0;

Graph::Graph(QString const& name)
    : mName(name)
{
    mID = ++smMaxGraphID;
}
