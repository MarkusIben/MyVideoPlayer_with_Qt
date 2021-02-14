#include "PlaylistWidget.h"
#include "ui_PlaylistWidget.h"

PlaylistWidget::PlaylistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistWidget)
{
    ui->setupUi(this);
}

PlaylistWidget::~PlaylistWidget()
{
    delete ui;
}

void PlaylistWidget::setCurrentIndex(const int &index)
{
    ui->listPlaylist->setCurrentRow(index);
}

void PlaylistWidget::addToPlaylist(const QString &fileName)
{
    ui->listPlaylist->addItem(fileName);
    ui->lblItemCounter->setText("Media counter: " + QString::number(ui->listPlaylist->count()));
}

void PlaylistWidget::clearPlaylist()
{
    ui->listPlaylist->clear();
}

void PlaylistWidget::removeFromPlaylist(const int &index)
{
    delete ui->listPlaylist->takeItem(index);
}

void PlaylistWidget::on_btnClose_clicked()
{
    this->setVisible(false);
}

void PlaylistWidget::on_btnMoveUp_clicked()
{
    int currentIndex = ui->listPlaylist->currentRow();
    if (ui->listPlaylist->item(currentIndex)->isSelected() && currentIndex > 0){
        QListWidgetItem *currentItem = ui->listPlaylist->takeItem(currentIndex);
        ui->listPlaylist->insertItem(currentIndex - 1, currentItem);
        ui->listPlaylist->setCurrentRow(currentIndex - 1);
        emit moveUpItemInPL(currentIndex);                      //emit signal for change in playlist
                                                                //change entry place in list widget with "takeItem" and "insertItem"
    }
}

void PlaylistWidget::on_btnMoveDown_clicked()
{
    int currentIndex = ui->listPlaylist->currentRow();
    if (ui->listPlaylist->item(currentIndex)->isSelected() && currentIndex < (ui->listPlaylist->count() - 1)){
        QListWidgetItem *currentItem = ui->listPlaylist->takeItem(currentIndex);
        ui->listPlaylist->insertItem(currentIndex + 1, currentItem);
        ui->listPlaylist->setCurrentRow(currentIndex + 1);
        emit moveDownItemInPL(currentIndex);                    //emit signal for change in playlist
                                                                //change entry place in list widget with "takeItem" and "insertItem"
    }
}

void PlaylistWidget::on_btnAdd_clicked()
{
    emit addItemInPL();
}

void PlaylistWidget::on_btnRemove_clicked()
{
    QVector<int> indexes;                                       //for saving the indexes of all selected clips, create a QVector
    for (int i = (ui->listPlaylist->count() - 1); i >= 0; i--){ //go through the whole list widget (but start at the end!...
                                                                //...because items will be removed, which will changes the higher indexes)...
        if (ui->listPlaylist->item(i)->isSelected()){           //...and each item that is selcted..
            indexes.append(i);                                  //...will be added to the QVector..
            delete ui->listPlaylist->takeItem(i);               //...afterward delete the clip entry in the list widget
            //(Qt "takeItem": "Items removed from a list widget will not be managed by Qt, and will need to be deleted manually.")
        }
    }
    ui->lblItemCounter->setText("Media counter: " + QString::number(ui->listPlaylist->count()));
    emit removeItemsInPL(indexes);
}

void PlaylistWidget::on_listPlaylist_doubleClicked(const QModelIndex &index)
{
    emit doubleClickInPL(index.row());
}

void PlaylistWidget::on_listPlaylist_itemSelectionChanged()
{
    int tempSelectionCounter = 0;
    for (int i = 0; i < ui->listPlaylist->count(); i++){       //Alle Elemente in der msListBox durchgehen
        if (ui->listPlaylist->item(i)->isSelected()){
            tempSelectionCounter++;
        }
    }
    if (tempSelectionCounter > 1){
        ui->btnMoveUp->setEnabled(false);
        ui->btnMoveDown->setEnabled(false);
    }
    else {
        ui->btnMoveUp->setEnabled(true);
        ui->btnMoveDown->setEnabled(true);
    }
}
