#include "FilterFilesModel.h"

FilterFilesModel::FilterFilesModel(QObject *parent)
    :QFileSystemModel(parent)
{

}

FilterFilesModel::~FilterFilesModel()
{

}



QString FilterFilesModel::findFileFolder(const QString &dir,
                                         const QString &name)
{
    //создаем итератор для поиска файлов с требуемыми флагами
    QDirIterator it(dir, QDir::AllDirs | QDir::Hidden | QDir::System| QDir::Files);

    while(it.hasNext())
    {
        it.next();
        if(it.fileName().contains(name))
        {
            //если имя файла содержит подстроку с введенными символами, то
            //возвращаем путь к этому файлу
            return it.fileInfo().filePath();
        }
    }
    return QString();
}
