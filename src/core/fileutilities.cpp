/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of utilites targeted to working with files
 */

#include <QDebug>
#include <QString>
#include <QFile>
#include <QDir>
#include <QPair>
#include "fileutilities.h"

using namespace RSE::Core;
using namespace RSE::Utilities;

//! Retrieve a pair consisted of a data object file and its type
QPair<AbstractDataObject::ObjectType, QSharedPointer<QFile>> File::getDataObjectFile(QString const& path, QString const& fileName)
{
    const QString kScalarFileName = "w1.prn";
    const QString kVectorFileName = "w3.prn";
    const QString kMatrixFileName = "w9.prn";
    const QString kSurfaceFileName = "xy.prn";
    std::function <bool(QString const&)> isNameEqual = [&fileName](QString const & name) { return !fileName.compare(name, Qt::CaseInsensitive); };
    AbstractDataObject::ObjectType type;
    if (isNameEqual(kScalarFileName))
        type = AbstractDataObject::ObjectType::kScalar;
    else if (isNameEqual(kVectorFileName))
        type = AbstractDataObject::ObjectType::kVector;
    else if (isNameEqual(kMatrixFileName))
        type = AbstractDataObject::ObjectType::kMatrix;
    else if (isNameEqual(kSurfaceFileName))
        type = AbstractDataObject::ObjectType::kSurface;
    else
        return {type, nullptr};
    QString filePath = path + QDir::separator() + fileName;
    QSharedPointer<QFile> pFile(new QFile(filePath));
    if (!pFile->open(QIODeviceBase::ReadOnly))
    {
        qWarning() << QString("Could not read a data object from the file %1").arg(filePath);
        return {type, nullptr};;
    }
    return {type, pFile};
}

//! Load all the content of a file
QString File::loadFileContent(QString const& path)
{
    QFile fileStyleSheet(path);
    QString content;
    if (!fileStyleSheet.exists())
    {
        qDebug() << QString("The file located at %1 was not found").arg(path) << Qt::endl;
    }
    else
    {
        fileStyleSheet.open(QFile::ReadOnly | QFile::Text);
        QTextStream streamStyle(&fileStyleSheet);
        content = streamStyle.readAll();
        fileStyleSheet.close();
    }
    return content;
}
