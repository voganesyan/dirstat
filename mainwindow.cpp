#include "mainwindow.h"
#include "directorychooser.h"
#include "directoryscanner.h"

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QSplitter(parent)
{
    DirectoryChooser* chooser = new DirectoryChooser(this);
    DirectoryScanner* scanner = new DirectoryScanner(this);
    connect(chooser, SIGNAL(choosenDirectoryChanged(const QString&)),
            scanner, SLOT(scanDirectory(const QString&)));
}

MainWindow::~MainWindow()
{

}
