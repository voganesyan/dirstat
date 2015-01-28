#ifndef DIRECTORYSCANNER_H
#define DIRECTORYSCANNER_H

#include <QWidget>
#include <mutex>
#include <atomic>
#include <thread>         // std::thread
class QFileInfo;
class QStandardItemModel;
class QTreeView;
class QLineEdit;
class QStatusBar;
class QTimer;
class QPushButton;

class DirectoryScanner : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryScanner(QWidget *parent = 0);

signals:

private slots:
    void on_lineedit_returnPressed();
    void updateModel ();
public slots:
    void scanDirectory(const QString &path);

private:

    typedef struct Group
    {
     QString name;//extension
     quint64 number;//number of files
     quint64 total_size;
    } Group;


    void add_to_groupslist(QFileInfo* info);
    void clear_grouplist();
    QString size2string(quint64 bytes);

QStandardItemModel *model;
QTreeView* list;
QLineEdit* lineedit;
QStatusBar* statusbar;
QPushButton* stopButton;
QTimer *timer;

QList<Group*> groups;

std::thread thread;
std::mutex mutex;
std::atomic<bool> thread_kill_youself_please;
std::atomic<bool> thread_is_alive;
};


#endif // DIRECTORYSCANNER_H
