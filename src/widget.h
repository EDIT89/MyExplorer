#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "FileSystemModel.h"
#include "FilterFilesModel.h"
#include <QItemSelectionModel>
#include <QDebug>


class FileSystemModel;
class FilterFilesModel;
class QItemSelectionModel;


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


public slots:
    void slotTreeViewClicked(const QModelIndex &index);
    void slotEditPathLine_EnterPressed();
    void slotSearchEditLine_EnterPressed();

private:
    void moveToFile(const QString &path);
    Ui::Widget *ui;
    FileSystemModel *fileSystemModel;
    FilterFilesModel *filterFilesModel;
    QItemSelectionModel *selectionModel;
    QItemSelectionModel *selectionModelTree;
    QFileInfo fileInfo;
};
#endif // WIDGET_H
