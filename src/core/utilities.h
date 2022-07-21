/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of utilities
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSharedPointer>
#include "abstractdataobject.h"

class QFile;
class QString;

namespace RSE
{

namespace Utilities
{

namespace File
{

QPair<Core::AbstractDataObject::ObjectType, QSharedPointer<QFile>> getDataObjectFile(QString const& path, QString const& fileName);
QString loadFileContent(QString const& path);

}

}

}

#endif // UTILITIES_H
