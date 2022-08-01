/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of utilities targeted to working with files
 */

#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <QSharedPointer>
#include "abstractdataobject.h"

class QFile;
class QString;

namespace RSE::Utilities::File
{

QPair<Core::AbstractDataObject::ObjectType, QSharedPointer<QFile>> getDataObjectFile(QString const& path, QString const& fileName);
QString loadFileContent(QString const& path);

}

#endif // FILEUTILITIES_H
