#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplitter>


class MainWindow : public QSplitter
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
