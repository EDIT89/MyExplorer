#include "widget.h"
#include "ui_widget.h"
#include "FileSystemModel.h"
#include "FilterFilesModel.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //создаем объект модели файловой системы
    fileSystemModel = new FileSystemModel(this);

    //создаем объект модели для выбора элемента в TreeView
    selectionModelTree = new QItemSelectionModel(fileSystemModel, this);

    //устанавливаем стартовую директорию домашней(по заданию)
    fileSystemModel->setRootPath(QDir::homePath());
    //устанавливаем необходимы фильтры отображения информации
    fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot |
                               QDir::Hidden | QDir::Files);
    //загружаем можедь в treeView
    ui->treeView->setModel(fileSystemModel);
    ui->treeView->setSelectionModel(selectionModelTree);

    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);

    ui->treeView->setRootIndex(fileSystemModel->index(
                                   fileSystemModel->rootPath()));
    //в виджете QLineEdit устанавливаем путь до домашней директории
    ui->pathLineEdit->setText(QDir::homePath());

    //создаем объект модели файловой системы для ListView
    filterFilesModel = new FilterFilesModel(this);
    //создаем объект модели для выбора элемента в ListView
    selectionModel = new QItemSelectionModel(filterFilesModel, this);


    //устанавливаем стартовую директорию как в TreeView
    filterFilesModel->setRootPath(fileSystemModel->rootPath());

    //устанавливаем необходимы фильтры отображения информации
    filterFilesModel->setFilter(QDir::NoDotAndDotDot |
                                QDir::Hidden | QDir::Files | QDir::AllDirs);

    //устанавливаем модель для Listview
    ui->listView->setModel(filterFilesModel);

    ui->listView->setSelectionModel(selectionModel);

    ui->listView->setRootIndex(filterFilesModel->index(
                                   filterFilesModel->rootPath()));

    ui->searchLineEdit->setPlaceholderText("Имя папки или файла");

    //соединяем сигнал нажатия на элемент дерева со слотом
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotTreeViewClicked(QModelIndex)));

    //соединяем сигнал нажатия на кнопку Enter в pathLineEdit со слотом
    connect(ui->pathLineEdit, SIGNAL(returnPressed()), this,
            SLOT(slotEditPathLine_EnterPressed()));

    //соединяем сигнал нажатия на кнопку Enter в searchLineEdit со слотом
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this,
            SLOT(slotSearchEditLine_EnterPressed()));
}




Widget::~Widget()
{
    delete selectionModel;
    delete filterFilesModel;
    delete selectionModelTree;
    delete fileSystemModel;
    delete ui;
}




void Widget::slotTreeViewClicked(const QModelIndex &index)
{
    // по индексу модели получаем абсолютный путь до выбранной директории
    // и устанавливем этот путь в QLineEdit
    fileInfo  = fileSystemModel->fileInfo(index);
    ui->pathLineEdit->setText(fileInfo.absoluteFilePath());

    //устанавливаем путь для просмотра файлов в Listview
    filterFilesModel->setRootPath(fileInfo.absoluteFilePath());
    ui->listView->setRootIndex(filterFilesModel->index(filterFilesModel->rootPath()));
}




void Widget::slotEditPathLine_EnterPressed()
{
    if(!ui->pathLineEdit->text().isEmpty())//проверяем не пустая ли строка ввода
    {
        //перемещаемся по введенному пути
        moveToFile(ui->pathLineEdit->text());
    }
}




void Widget::slotSearchEditLine_EnterPressed()
{

    if(!ui->searchLineEdit->text().isEmpty())//проверяем не пустая ли строка ввода
    {
        //считываем выбраннную директорию
        QString currentDir = filterFilesModel->rootPath();
        //ищем файл с именем похожим на введенное
        QString newFilePath = filterFilesModel->findFileFolder(currentDir,
                                                               ui->searchLineEdit->text());
        if(!newFilePath.isEmpty())
        {
            //устанавливаем новый стартовый индекс для отображения
            ui->listView->setCurrentIndex(filterFilesModel->index(newFilePath));

            //выделяем результат и перемещаем его в поле зрения
            auto currentIndex = ui->listView->currentIndex();
            selectionModel->setCurrentIndex(currentIndex, QItemSelectionModel::Select);
        }
    }
}



void Widget::moveToFile(const QString &path)
{
    QString pathStr = path;
    //делим путь к файлу на подстроки
    QStringList pathStrList = pathStr.split(QRegExp("/"));

    QString ret;
    //идем в цикле по подстрокам и поочердно открываем ветки файловой системы
    for(int i = 1; i<pathStrList.count(); i++)
    {

        if(i==1)
        {
            //первой подстроке необходимо добавить / чтобы система правильно распознавала путь
            ret = "/" +ret + pathStrList.at(i)+"/";
            //устанавливаем текущий индекс модели по части пути
            ui->treeView->setCurrentIndex(fileSystemModel->index(ret));
            auto currentIndex = ui->treeView->currentIndex();// получаем текущий индекс
            //делаем выбор по текущему индексу
            selectionModelTree->select(QItemSelection(currentIndex,currentIndex),
                                       QItemSelectionModel::Select);
            //открываем часть ветки
            ui->treeView->setExpanded(currentIndex,true);

            filterFilesModel->setRootPath(ret);
            ui->listView->setRootIndex(filterFilesModel->index(filterFilesModel->rootPath()));
        }
        else
        {
            if(i != pathStrList.count()-1)
            {
                ret = ret + pathStrList.at(i)+"/";
                ui->treeView->setCurrentIndex(fileSystemModel->index(ret));
                auto currentIndex = ui->treeView->currentIndex();
                selectionModelTree->select(QItemSelection(currentIndex,currentIndex),
                                           QItemSelectionModel::Select);
                ui->treeView->setExpanded(currentIndex,true);

                filterFilesModel->setRootPath(ret);
                ui->listView->setRootIndex(filterFilesModel->index(filterFilesModel->rootPath()));
            }
            else
            {
                ret = ret + pathStrList.at(i);
                ui->treeView->setCurrentIndex(fileSystemModel->index(ret));
                auto currentIndex = ui->treeView->currentIndex();
                selectionModelTree->select(QItemSelection(currentIndex,currentIndex),
                                           QItemSelectionModel::Select);
                ui->treeView->setExpanded(currentIndex,true);

                filterFilesModel->setRootPath(ret);
                ui->listView->setRootIndex(filterFilesModel->index(filterFilesModel->rootPath()));

            }
        }
    }
}

