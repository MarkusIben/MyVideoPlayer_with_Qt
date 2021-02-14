#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

#include <QSlider>
#include <QFileDialog>
#include <QBoxLayout>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QKeyEvent>
#include <QRandomGenerator>

#include <ctime>
#include "PlaylistWidget.h"
#include "FullScreenWidget.h"
#include "ZoomVideoScreenWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //for menue signals:
    void on_actionOpenFile_triggered();
    void on_actionOpenDirectory_triggered();
    void on_actionExit_triggered();
    void on_actionPlay_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void on_actionNext_triggered();
    void on_actionPrevious_triggered();
    void on_actionRandom_triggered(bool checked);
    void on_actionFullscreen_triggered();
    void on_actionQuizMode_triggered(bool checked);

    //for button signals:
    void on_btnOpenFile_clicked();
    void on_btnPlayPause_clicked();
    void on_btnStop_clicked();
    void on_btnOpenFolder_clicked();
    void on_btnPlaylist_clicked();
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();
    void on_btnPlaylistShuffle_clicked(bool checked);
    void on_btnFullscreen_clicked();
    void on_btnExit_clicked();

    //---------- own connected signals: ----------

    //for player signals:
    void on_videoDuration_changed();
    void on_videoPosition_changed();
    void on_playerMediaStatus_changed(QMediaPlayer::MediaStatus mediaStatus);
    void on_playerState_changed(QMediaPlayer::State state);
    void on_player_error(QMediaPlayer::Error error);

    //for playlist signals:
    void on_playlistMedia_inserted();
    void on_playlistMedia_removed();
    void on_playlistCurrentIndex_changed(int position);
    void on_playlistLoad_failed();

    //for video slider signals:
    void on_sliderVideoPos_clickSliderMoved(double mouseXPosOnSliderInPercent);
    void on_sliderVideoPos_clickSliderClicked(double mouseXPosOnSliderInPercent);
    void on_sliderVideoPos_clickSliderReleased();

    //for playlist widget:
    void on_btnMoveUp_clicked(int selectedIndex);
    void on_btnMoveDown_clicked(int selectedIndex);
    void on_btnAdd_clicked();
    void on_btnRemove_clicked(QVector<int> indexes);
    void on_doubleClick_inPL(int index);

    //for volume slider signals:
    void on_sliderVolPos_clickSliderMoved(double mouseXPosOnSliderInPercent);

    //for full screen signals:
    void on_fullScreen_doubleClick();
    void on_fullScreen_mousePress();



private:
    //ui
    Ui::MainWindow *ui;
    int videoSliderMaxValue = 10000;
    QHBoxLayout* widgetMidAreaLayout = nullptr;

    //player
    QMediaPlayer* player = nullptr;
    bool playerWasPlayingBeforeSliderClicked = false;
    qint64 videoDuration = 0;               //for calculating slider position

    //screen
    FullScreenWidget* fullScreenWidget = nullptr;
    ZoomVideoScreenWidget* zoomVideoWidget = nullptr;

    //playlist
    QMediaPlaylist* playlist = nullptr;
    PlaylistWidget* playlistWidget = nullptr;

    //quiz
    bool quizMode = false;
    bool quizNewClip = false;
    int quizShowTimeMs = 1000;
    qint64 quizVideoStartPos = 0;
    qint64 quizVideoPausePos = 0;



    void init();

    void openFiles();
    void openDir();

    void setupNewPlaylistAndPlay(const QStringList &filePaths);
    void addToPlaylist();
    void addToPlaylist(const QStringList &filePaths);
    void moveInPlaylist(const int &selectedIndex, const int &direction);
    void setCurrentPlaylistIndex(const int &index);
    void clearPlaylist();

    void setPlayerState(const QMediaPlayer::State &state);

    void play();

    bool eventFilter(QObject* sender, QEvent* event) override;
};

#endif // MAINWINDOW_H
