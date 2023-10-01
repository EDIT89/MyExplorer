#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel:public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FileSystemModel(QObject *parent = nullptr);
    virtual ~FileSystemModel();

};

#endif // FILESYSTEMMODEL_H
