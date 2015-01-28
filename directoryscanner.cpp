#include "directoryscanner.h"
#include <QStandardItemModel>
#include <QDirIterator>
#include <QLineEdit>
#include <QTreeView>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
DirectoryScanner::DirectoryScanner(QWidget *parent) :
    QWidget(parent)
{
    QStringList labels;
    labels << "Group" << "Number of files" << "Total size" << "Average size";
    model = new QStandardItemModel();
    model->setHorizontalHeaderLabels (labels);

    list = new QTreeView();
    QHeaderView* header = list->header();
   // header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionsClickable(true);
   // header->setSortIndicatorShown(true);
    //list->setModel (model);

    lineedit = new QLineEdit;
    connect(lineedit, SIGNAL(returnPressed()),
                     this,   SLOT(on_lineedit_returnPressed()));

    statusbar = new QStatusBar();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
                 this, &DirectoryScanner::updateModel );

    stopButton = new QPushButton("Stop");
    QObject::connect(stopButton, &QPushButton::clicked,
                     [this]() { thread_kill_youself_please = true; });

    thread_kill_youself_please = false;
    thread_is_alive = false;

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel("Directory"));
    hbox->addWidget(lineedit);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addLayout(hbox);
    layout->addWidget(list);
    layout->addWidget(statusbar);
    setLayout( layout );
}



void DirectoryScanner::scanDirectory(const QString& path)
{
   lineedit->setText(path);
   emit lineedit->returnPressed();
}

#include <unistd.h>
void DirectoryScanner::on_lineedit_returnPressed()
{
    QLineEdit *lineedit = qobject_cast<QLineEdit *>(QObject::sender());

     //bad approach(use ?futures? in the future)
     if(thread.joinable()) //thread has already run
     {
        thread_kill_youself_please = true;
        thread.join();
     }

    statusbar->showMessage("Scanning directory...");
    statusbar->addPermanentWidget(stopButton);
    stopButton->show();

    clear_grouplist();
    model->removeRows(0, model->rowCount());
    thread_kill_youself_please = false;

    QString path = lineedit->text();

    auto scan = [this] (const QString& path)
    {
        thread_is_alive = true;

        QDirIterator it (path, QDir::Files, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            if (thread_kill_youself_please == true)//somebody wants to kill me
                break;
            QFileInfo file = it.fileInfo();
            add_to_groupslist(&file);
            it.next();
        }

        thread_is_alive = false;
    };

    thread = std::thread(scan, path);


    timer->start(500);
}

void DirectoryScanner::add_to_groupslist(QFileInfo* info)
{
    QString name = info->suffix();

    Group* group = 0;

    mutex.lock();
    for (int i = 0; i < groups.size(); ++i)
    {
        if(name == groups.at(i)->name)
        {
            group = groups[i];
            break;
        }
    }

    if (!group)
    {
       group = new Group;
       group->name = name;
       group->number = 0;
       group->total_size  = 0;
       groups.append(group);
    }
     group->number++;
     group->total_size += info->size();

    mutex.unlock();
}

void DirectoryScanner::clear_grouplist()
{
    mutex.lock();
    for (int i = 0; i < groups.size(); ++i)
        delete groups[i];

    groups.clear();
    mutex.unlock();
}

void DirectoryScanner::updateModel ()
{
    if (thread_is_alive == false)
    {
      timer->stop();
      statusbar->showMessage("Scanning complete.");
      statusbar->removeWidget(stopButton);
    }
    list->setModel (0);
    model->removeRows(0, model->rowCount());

    mutex.lock();
    for (int i = 0; i < groups.size(); ++i)
    {

        QList<QStandardItem *> items;

        items << new QStandardItem (groups[i]->name)
              << new QStandardItem (QString::number(groups[i]->number))
              << new QStandardItem (size2string(groups[i]->total_size))
              << new QStandardItem (size2string(groups[i]->total_size/(float)groups[i]->number));

        model->appendRow (items);
    }
    mutex.unlock();
    list->setModel (model);
}

#include <QFileSystemModel>
QString DirectoryScanner::size2string (quint64 bytes)
{
    // According to the Si standard KB is 1000 bytes, KiB is 1024
    // but on windows sizes are calulated by dividing by 1024 so we do what they do.
    const float kb = 1024;
    const float mb = 1024 * kb;
    const float gb = 1024 * mb;
    const float tb = 1024 * gb;

    if (bytes >= tb)
        return QFileSystemModel::tr("%1 TB").arg(QLocale().toString(bytes / tb, 'f', 3));
    if (bytes >= gb)
        return QFileSystemModel::tr("%1 GB").arg(QLocale().toString(bytes / gb, 'f', 2));
    if (bytes >= mb)
        return QFileSystemModel::tr("%1 MB").arg(QLocale().toString(bytes / mb, 'f', 1));
    if (bytes >= kb)
        return QFileSystemModel::tr("%1 KB").arg(QLocale().toString(bytes / kb));
    return QFileSystemModel::tr("%1 byte(s)").arg(QLocale().toString(bytes));
}
