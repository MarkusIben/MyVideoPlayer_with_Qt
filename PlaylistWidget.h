#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QListWidgetItem>

namespace Ui {
class PlaylistWidget;
}

class PlaylistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistWidget(QWidget *parent = nullptr);
    ~PlaylistWidget();

    void setCurrentIndex(const int &index);

    void addToPlaylist(const QString &fileName);
    void clearPlaylist();
    void removeFromPlaylist(const int &index);

signals:
    void moveUpItemInPL(int index);
    void moveDownItemInPL(int index);
    void addItemInPL();
    void removeItemsInPL(QVector<int> indexes);
    void doubleClickInPL(int index);

private slots:
    void on_btnClose_clicked();
    void on_btnMoveUp_clicked();
    void on_btnMoveDown_clicked();
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_listPlaylist_doubleClicked(const QModelIndex &index);
    void on_listPlaylist_itemSelectionChanged();

private:
    Ui::PlaylistWidget *ui;

};

#endif // PLAYLISTWIDGET_H
