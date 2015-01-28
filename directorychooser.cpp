#include "directorychooser.h"
#include <QComboBox>
#include <QStandardItemModel>
#include <QTreeView>
#include <QFileInfoList>
#include <QDirIterator>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>

DirectoryChooser::DirectoryChooser(QWidget *parent) :
    QWidget(parent)
{
    model = new QStandardItemModel();
    QStringList labels;
    labels <<"Dirs" << "Number of subdirs";
    model->setHorizontalHeaderLabels (labels);

    QTreeView* tree = new QTreeView();
    tree->setModel(model);
    connect(tree, &QTreeView::expanded,
            this, &DirectoryChooser::populate_tree);
    connect(tree->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &DirectoryChooser::on_tree_selectionChanged);

    QComboBox* comboboxRoot = new QComboBox();
    //we use old syntax because currentIndexChanged SIGNAL is overloaded
    //and we don't have enough time to find the better way
    connect(comboboxRoot, SIGNAL(currentIndexChanged(int )),
            this,   SLOT(on_comboboxRoot_currentIndexChanged(int )));

    QFileInfoList drives = QDir::drives();
    for (int i = 0; i < drives.size(); ++i) {
        QFileInfo fileInfo = drives.at(i);
        comboboxRoot->addItem(fileInfo.filePath());
    }

    statusbar = new QStatusBar();

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel("Drive"));
    hbox->addWidget(comboboxRoot);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addLayout(hbox);
    layout->addWidget(tree);
    layout->addWidget(statusbar);
    setLayout( layout );
}

void DirectoryChooser::add_to_tree(QStandardItem *parentItem)
{
  //If parentItem already has children, then remove them first
  parentItem->removeRows(0, parentItem->rowCount());

  QDirIterator it (parentItem->data().toString(), QDir::Dirs | QDir::NoDotAndDotDot);
  int subdirs = 0;

  while (it.hasNext()) {
     it.next();
     QFileInfo fileInfo = it.fileInfo();
     QStandardItem *item = new QStandardItem(fileInfo.fileName());
     item->setData(fileInfo.absoluteFilePath());
     QStandardItem* item_subdirnumber  = new QStandardItem ("?");
     QList<QStandardItem*> items;
     items << item << item_subdirnumber;

     parentItem->appendRow(items);
     subdirs++;
  }

  QModelIndex index = model->indexFromItem(parentItem);
  QModelIndex sibling = index.sibling(index.row(),1);
  model->itemFromIndex(sibling)->setText(QString::number(subdirs));
}

void DirectoryChooser::populate_tree(const QModelIndex &parent)
{
  QStandardItem* parentItem = model->itemFromIndex(parent);
  statusbar->showMessage("Directory branch building...");

  for (int i = 0; i < parentItem->rowCount(); i++)
  {
       QStandardItem* child = parentItem->child(i);
       add_to_tree(child);
  }

  statusbar->showMessage("Directory branch has built.", 500);
}

void DirectoryChooser::on_tree_selectionChanged(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/)
{
    QItemSelectionModel *selectionModel = qobject_cast<QItemSelectionModel *>(QObject::sender());
    const QModelIndex index = selectionModel->currentIndex();
    QStandardItem *item=model->itemFromIndex(index);

    if (item)
    emit choosenDirectoryChanged(item->data().toString());
}

void DirectoryChooser::on_comboboxRoot_currentIndexChanged(int index)
{
    model->removeRows(0, model->rowCount());

    QComboBox *combobox = qobject_cast<QComboBox *>(QObject::sender());
    QString path = combobox->itemText(index);

    QStandardItem* item = new QStandardItem (path);
    item->setData(path);

    QList<QStandardItem*> items;
    items << item << new QStandardItem ("?");

    model->appendRow (items);

    add_to_tree(item);
}
