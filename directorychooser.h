#ifndef DIRECTORYCHOOSER_H
#define DIRECTORYCHOOSER_H

#include <QWidget>
class QItemSelection;
class QModelIndex;
class QStandardItem;
class QStandardItemModel;
class QStatusBar;

class DirectoryChooser : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryChooser(QWidget *parent = 0);

signals:
void choosenDirectoryChanged(const QString& path);

private slots:
    void on_comboboxRoot_currentIndexChanged(int index);
    void on_tree_selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void populate_tree(const QModelIndex &parent);

private:
    void add_to_tree(QStandardItem *parentItem);

QStandardItemModel *model;
QStatusBar* statusbar;
};

#endif // DIRECTORYCHOOSER_H
