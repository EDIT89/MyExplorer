#ifndef FILTERFILESMODEL_H
#define FILTERFILESMODEL_H

#include <QFileSystemModel>
#include <QDir>
#include <QDebug>

class FilterFilesModel:public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FilterFilesModel(QObject *parent = nullptr);
    virtual ~FilterFilesModel();

    QString findFileFolder(const QString &dir,
                           const QString &name);

};

#endif // FILTERFILESMODEL_H
