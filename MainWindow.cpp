#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ClickableSlider.h"



// public:  -------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// private slots: ---------------------------------------------------------------------------------------------------------

// menue slots - - - - - - - - - - - - - - - - - - - - - - - -

void MainWindow::on_actionOpenFile_triggered()
{
    openFiles();
}

void MainWindow::on_actionOpenDirectory_triggered()
{
    openDir();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionPlay_triggered()
{
    this->play();
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
    if(quizMode){               //***for quiz mode
        quizNewClip = false;
    }
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
}

void MainWindow::on_actionNext_triggered()
{
    on_btnNext_clicked();
}

void MainWindow::on_actionPrevious_triggered()
{
    on_btnPrevious_clicked();
}

void MainWindow::on_actionRandom_triggered(bool checked)
{
    on_btnPlaylistShuffle_clicked(checked);
}

void MainWindow::on_actionFullscreen_triggered()
{
    on_btnFullscreen_clicked();
}

void MainWindow::on_actionQuizMode_triggered(bool checked)
{
    //***for quiz mode
    quizMode = checked;
    quizNewClip = checked;
    on_btnPlaylistShuffle_clicked(checked);
    ui->btnPlaylistShuffle->setEnabled(!checked);
}


// buttons slots - - - - - - - - - - - - - - - - - - - - - - -

void MainWindow::on_btnOpenFile_clicked()
{
    openFiles();
}

void MainWindow::on_btnOpenFolder_clicked()
{
    openDir();
}

void MainWindow::on_btnPlayPause_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState){
        player->pause();
        if(quizMode){
            quizNewClip = false;
        }
    }
    else{
        this->play();
    }
}

void MainWindow::on_btnStop_clicked()
{
    player->stop();
}

void MainWindow::on_btnPlaylist_clicked()
{
    if (playlistWidget->isVisible()){
        playlistWidget->setVisible(false);
    }
    else {
        playlistWidget->setVisible(true);
    }
}

void MainWindow::on_btnPrevious_clicked()
{
    playlist->previous();
}

void MainWindow::on_btnNext_clicked()
{
    playlist->next();
}

void MainWindow::on_btnPlaylistShuffle_clicked(bool checked)
{
    if(checked){
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else {
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}


void MainWindow::on_btnFullscreen_clicked()
{
    fullScreenWidget->showFullScreen();

    widgetMidAreaLayout->removeWidget(zoomVideoWidget);

    fullScreenWidget->addWidgetToLayout(zoomVideoWidget);
}

void MainWindow::on_btnExit_clicked()
{
    close();
}



// own slots - - - - - - - - - - - - - - - - - - - - - -

// player slots -  -  -  -  -  -

void MainWindow::on_videoDuration_changed()                 //slot emitted by player (QMediaPlayer):
{
    videoDuration = player->duration();                     //save video duration into member variable
    int hour = static_cast<int>(videoDuration / 3600000);   //transfer video duration from ms to timeformat
    int min = static_cast<int>(videoDuration / 60000);
    int sec = static_cast<int>(videoDuration / 1000);
    QTime durationTime(hour, min, sec);
    ui->lblDuration->setText(durationTime.toString());      //display video duration as timeformat in label

    //***for quiz mode
    if(quizMode){
        quizNewClip = true;
        this->play();
    }
}

void MainWindow::on_videoPosition_changed()                 //When video position has changed (while playing or using the slider)...
{
    int newSliderPosition = 0;                              //...calculate new video slider position regarding the video duration...
    if (videoDuration != 0){                                //...but mathematicaly only possible when video duration is more then 0
        newSliderPosition =  static_cast<int>((player->position() * videoSliderMaxValue / videoDuration) );
    }
    ui->sliderVideoPos->setSliderPosition(newSliderPosition); //set slider position

    int hour = static_cast<int>(player->position() / 3600000);
    int min = static_cast<int>(player->position() / 60000); //transfer video position from ms to timeformat
    int sec = static_cast<int>(player->position() / 1000);
    QTime positionTime(hour, min, sec);
    ui->lblPosition->setText(positionTime.toString());      //display video position as timeformat in label


    //***for quiz mode
    if(quizNewClip){
        if(player->position() >= (quizVideoStartPos + quizShowTimeMs)){
            player->pause();
            quizNewClip = false;
        }
    }



}

void MainWindow::on_playerMediaStatus_changed(QMediaPlayer::MediaStatus mediaStatus)
{
    switch (mediaStatus) {
    case QMediaPlayer::UnknownMediaStatus:
        break;
    case QMediaPlayer::NoMedia:
        break;
    case QMediaPlayer::LoadingMedia:
        QApplication::setOverrideCursor(Qt::WaitCursor);
        ui->statusBar->showMessage("Video is loading...");
        break;
    case QMediaPlayer::LoadedMedia:
        break;
    case QMediaPlayer::StalledMedia:                        //*** maybe not even required
        QMessageBox::warning(this, this->windowTitle(), "Playback of the current media has stalled due to"
                                                        "insufficient buffering or some other temporary interruption.");
        break;
    case QMediaPlayer::BufferingMedia:
        break;
    case QMediaPlayer::BufferedMedia:
        ui->btnPlayPause->setEnabled(true);
        ui->btnStop->setEnabled(true);

        QApplication::restoreOverrideCursor();
        if(player->state() == QMediaPlayer::PlayingState){
            ui->statusBar->showMessage("Video is playing...");
        }
        break;
    case QMediaPlayer::EndOfMedia:
        ui->statusBar->showMessage("Video finished...");
        break;
    case QMediaPlayer::InvalidMedia:                        //error will be displayed with slot "on_player_error(...)"!
        break;
    }
//    QMediaPlayer::UnknownMediaStatus	0	The status of the media cannot be determined.
//    QMediaPlayer::NoMedia             1	The is no current media. The player is in the StoppedState.
//    QMediaPlayer::LoadingMedia    	2	The current media is being loaded. The player may be in any state.
//    QMediaPlayer::LoadedMedia         3	The current media has been loaded. The player is in the StoppedState.
//    QMediaPlayer::StalledMedia        4	Playback of the current media has stalled due to insufficient buffering or some other temporary interruption. The player is in the PlayingState or PausedState.
//    QMediaPlayer::BufferingMedia      5	The player is buffering data but has enough data buffered for playback to continue for the immediate future. The player is in the PlayingState or PausedState.
//    QMediaPlayer::BufferedMedia       6	The player has fully buffered the current media. The player is in the PlayingState or PausedState.
//    QMediaPlayer::EndOfMedia          7	Playback has reached the end of the current media. The player is in the StoppedState.
//    QMediaPlayer::InvalidMedia        8	The current media cannot be played. The player is in the StoppedState.
}

void MainWindow::on_playerState_changed(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        ui->btnPlayPause->setText("PLAY");
        ui->btnPlayPause->setFocus();
        ui->statusBar->showMessage("Video has been stoped");
        break;
    case QMediaPlayer::PlayingState:
        ui->btnPlayPause->setText("PAUSE");
        ui->btnPlayPause->setFocus();
        if(player->mediaStatus() == QMediaPlayer::BufferedMedia){
            ui->statusBar->showMessage("Video is playing...");
        }
        break;
    case QMediaPlayer::PausedState:
        ui->btnPlayPause->setText("PLAY");
        ui->btnPlayPause->setFocus();
        ui->statusBar->showMessage("Video has been paused");
        break;
    }
//    QMediaPlayer::StoppedState	0	The media player is not playing content, playback will begin from the start of the current track.
//    QMediaPlayer::PlayingState	1	The media player is currently playing content.
//    QMediaPlayer::PausedState     2	The media player has paused playback, playback of the current track will resume from the position the player was paused at.
}

void MainWindow::on_player_error(QMediaPlayer::Error error)
{
    QString errorMsg;
    switch (error) {
    case QMediaPlayer::ResourceError:
        if(player->mediaStatus() == QMediaPlayer::InvalidMedia){
            errorMsg = "The current media cannot be find or played.";
        }
        else {
            errorMsg = "The media resource couldn't be resolved. Check your codecs!";        }
        break;
    case QMediaPlayer::FormatError:
        errorMsg = "The format of a media resource isn't (fully) supported. ";
        errorMsg += "Playback may still be possible, but without an audio or video component.";
        break;
    case QMediaPlayer::AccessDeniedError:
        errorMsg = "There are not the appropriate permissions to play a media resource.";
        break;
    case QMediaPlayer::ServiceMissingError:
        errorMsg = "A valid playback service was not found, playback cannot proceed.";
        break;
    default:
        errorMsg = "Unknown playback error";
    }

    int tempIndex = playlist->currentIndex();
    int msgValue = QMessageBox::Yes;

    if(playlist->mediaCount() > 2){
        msgValue = QMessageBox::question(this, this->windowTitle(), errorMsg +
                                             "\n\nDo you want to remove the file from the playlist?",
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    }
    else {
        QMessageBox::critical(this, this->windowTitle(), errorMsg);
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }

    if(msgValue == QMessageBox::Yes){
        playlist->removeMedia(tempIndex);                   //remove media from playlist...
        playlistWidget->removeFromPlaylist(tempIndex);      //...and remove list entry in playlist widget
        playlist->previous();                               //...and before go on playing jump one back in playlist, because after the error the playlist
    }                                                       //would go on at the next index, but because of removing everything next has a new index (with -1)
}


// playlist slots -  -  -  -  -  -

void MainWindow::on_playlistMedia_inserted()
{
    ui->btnPlaylist->setEnabled(true);
    if(playlist->mediaCount() > 1){                         //if there's more then 1 media in playlist -> enable next/previous buttons
        ui->btnNext->setEnabled(true);
        ui->btnPrevious->setEnabled(true);
    }
    if(playlist->mediaCount() > 2){                         //if there's more then 2 media in playlist -> enable shuffle button
        ui->btnPlaylistShuffle->setEnabled(true);
    }
}

void MainWindow::on_playlistMedia_removed()
{
    if(playlist->mediaCount() < 1){                         //if there's no media in playlist -> disable play/pause/stop buttons
        ui->btnPlayPause->setEnabled(false);
        ui->btnStop->setEnabled(false);
    }
    if(playlist->mediaCount() < 2){                         //if there's less than 2 media in playlist -> disable next/previous buttons
        ui->btnNext->setEnabled(false);
        ui->btnPrevious->setEnabled(false);
    }
    if(playlist->mediaCount() < 3){                         //if there's less than 3 media in playlist -> disable shuffle button
        ui->btnPlaylistShuffle->setEnabled(false);
    }
}

void MainWindow::on_playlistCurrentIndex_changed(int position)
{
    playlistWidget->setCurrentIndex(position);              //When changing media in playlist, change index in the playlist widget...
    zoomVideoWidget->resetZoom();                           //...and reset the zoom    

}

void MainWindow::on_playlistLoad_failed()
{
    QMessageBox::warning(this, "Playlist", "Playlist could not be loaded:\n" + playlist->errorString());        //*** maybe not required
}


//  video slider slots -  -  -  -  -  -

void MainWindow::on_sliderVideoPos_clickSliderMoved(double mouseXPosOnSliderInPercent)
{
    qint64 newPositionInMs = static_cast<qint64>(mouseXPosOnSliderInPercent * videoDuration);
    player->setPosition(newPositionInMs);                   //set new player position using the relative
                                                            //mouse position on the slider in percent (0-1)
}

void MainWindow::on_sliderVideoPos_clickSliderClicked(double mouseXPosOnSliderInPercent)
{
    if(player->state() == QMediaPlayer::PlayingState){      //if Player is playing while clicking on slider...
        playerWasPlayingBeforeSliderClicked = true;         //...save state in global bool...
    }
    player->pause();                                        //...then pause player for fluent slider movement...
    qint64 newPositionInMs = static_cast<qint64>(mouseXPosOnSliderInPercent * videoDuration);
    player->setPosition(newPositionInMs);                   //...and set new player position using the relative
                                                            //mouse position on the slider in percent (0-1)
}

void MainWindow::on_sliderVideoPos_clickSliderReleased()
{
    if(playerWasPlayingBeforeSliderClicked){                //...keep on playing when it did play before clicking
        this->play();
        playerWasPlayingBeforeSliderClicked = false;        //...and set relating bool on the default "false" back again
    }
}

//  playlist widget slots -  -  -  -  -  -

void MainWindow::on_btnMoveUp_clicked(int selectedIndex)
{
    moveInPlaylist(selectedIndex, -1);
}

void MainWindow::on_btnMoveDown_clicked(int selectedIndex)
{
    moveInPlaylist(selectedIndex, +1);
}

void MainWindow::on_btnAdd_clicked()
{
    addToPlaylist();
}

void MainWindow::on_btnRemove_clicked(QVector<int> indexes)
{
    if (!indexes.isEmpty()){                                //if any clip was selected before pressing the remove button
        int currentPlaylistIndex = playlist->currentIndex();    //save current playlist index, player position and player state...
        qint64 tempPlayerPosition = player->position();         //...for the case, that the current clip will moved in the playlist...
        QMediaPlayer::State tempPlayerState = player->state();  //...because of removing clips with lower indexes from the playlistthe
        for(const int &index : indexes){                        //go through the Qvector of selected indexes in the playlist widget (from high to low)
            if (index < currentPlaylistIndex){                      //if at least one of the seleted indexes is lower than the current played one
                player->pause();                                        //pause the player and reduce the current index by 1...
                currentPlaylistIndex--;                                 //...so it will fit again after...
            }
            playlist->removeMedia(index);                           //...remove the media at the selected indexin the playlist
        }                                                           //(note: items from playlistWidget already in it's own class method deleted)

        if (currentPlaylistIndex != playlist->currentIndex()){  //if the current played index needed to be reduced in the for loop...
            playlist->setCurrentIndex(currentPlaylistIndex);        //...set the adapted playlist index...
            player->setPosition(tempPlayerPosition);                //...and the saved player position...
            setPlayerState(tempPlayerState);                        //...and set the player in the playing state as it was before

        }
        playlistWidget->setCurrentIndex(currentPlaylistIndex);  //at this point also update the current index in the playlist widget
    }
}

void MainWindow::on_doubleClick_inPL(int index)
{

    playlist->setCurrentIndex(index);
    this->play();
}


//  volume slider slots -  -  -  -  -  -

void MainWindow::on_sliderVolPos_clickSliderMoved(double mouseXPosOnSliderInPercent)
{
    int newVolume = static_cast<int>(mouseXPosOnSliderInPercent * 100);
    ui->lblVolPercent->setText(QString::number(newVolume) + " %");
    ui->sliderVol->setValue(newVolume);
    player->setVolume(newVolume);
}

//  full screen slots -  -  -  -  -  -

void MainWindow::on_fullScreen_doubleClick()                //close full screen on double click
{
    fullScreenWidget->close();
    bool playlistWasVisible = playlistWidget->isVisible();  //remember if playlist was visible before...
    widgetMidAreaLayout->removeWidget(playlistWidget);      //...remove playlist from mid area...
    widgetMidAreaLayout->addWidget(zoomVideoWidget);        //...then first add video screen to mid area (to have it on the left side)...
    widgetMidAreaLayout->addWidget(playlistWidget);         //...and then add playlist again to mid area (to have it on the right side)...
    playlistWidget->setVisible(playlistWasVisible);         //...and make the playlist visible or not like it was before
}

void MainWindow::on_fullScreen_mousePress()
{
    on_fullScreen_doubleClick();
}


// private: ---------------------------------------------------------------------------------------------------------


void MainWindow::init()
{                                                           //INITIALIZE:
    player = new QMediaPlayer(this);                        //media player, who handles (play, pause, stop) video (or audio) files, one at a time
    zoomVideoWidget = new ZoomVideoScreenWidget(this);      //own class: contains a QVideoWidget (the screen for the player), which can be zoomed!
    fullScreenWidget = new FullScreenWidget(nullptr);       //own class: for showing the zoomVideoWidget in fullscreen
    playlist = new QMediaPlaylist(this);                    //playlist, where multiple media files can be stored and served to the player
    playlistWidget = new PlaylistWidget(ui->widgetMidArea); //own class: an widget which displayer the playlist and which can be hide or show


    // SETUP PLAYER ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    player->setVideoOutput(zoomVideoWidget->getVideoScreen());  //set video screen as player output
    player->setNotifyInterval(10);                          //raise notify interval (from default 1000ms) for fluid progressbar/slider

                                                            //connections for updating slider / progressbar and time labels
    connect(player, SIGNAL(durationChanged(qint64)),
        this, SLOT(on_videoDuration_changed()));
    connect(player, SIGNAL(positionChanged(qint64)),
        this, SLOT(on_videoPosition_changed()));
                                                            //connections to receive changes about media status, state and errors
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
        this, SLOT(on_playerMediaStatus_changed(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
        this, SLOT(on_playerState_changed(QMediaPlayer::State)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)),
        this, SLOT(on_player_error(QMediaPlayer::Error)));


    // SETUP PLAYLIST ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    player->setPlaylist(playlist);                          //...assign it to the player
    playlist->setPlaybackMode(QMediaPlaylist::Loop);        //...and set playback mode on loop

                                                            //when media in the playlist are inserted or removed, or the index changed
    connect(playlist, SIGNAL(mediaInserted(int, int)),
        this, SLOT(on_playlistMedia_inserted()));
    connect(playlist, SIGNAL(mediaRemoved(int, int)),
        this, SLOT(on_playlistMedia_removed()));
    connect(playlist, SIGNAL(currentIndexChanged(int)),
        this, SLOT(on_playlistCurrentIndex_changed(int)));


    // SETUP ZOOM_VIDEO_WIDGET ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    widgetMidAreaLayout = new QHBoxLayout;                  //create new layout for the mid area of the application...
    ui->widgetMidArea->setLayout(widgetMidAreaLayout);
    widgetMidAreaLayout->addWidget(zoomVideoWidget);        //...and add the videoScreen to in


    // SETUP FULL_SCREEN_WIDGET ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
                                                            //for mouse events on full screen view
    connect(fullScreenWidget, SIGNAL(on_doubleClick()),
        this, SLOT(on_fullScreen_doubleClick()));
    connect(fullScreenWidget, SIGNAL(on_mousePress()),
        this, SLOT(on_fullScreen_mousePress()));


    // SETUP PLAYLIST_WIDGET ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    widgetMidAreaLayout->addWidget(playlistWidget);         //...and add it to the mid area layout (beside the video screen)
    playlistWidget->setVisible(false);                      //...but hide at first

                                                            //for the playlist widget (buttons and mouse) and the playlist
    connect(playlistWidget, SIGNAL(moveUpItemInPL(int)),
        this, SLOT(on_btnMoveUp_clicked(int)));
    connect(playlistWidget, SIGNAL(moveDownItemInPL(int)),
        this, SLOT(on_btnMoveDown_clicked(int)));
    connect(playlistWidget, SIGNAL(addItemInPL()),
        this, SLOT(on_btnAdd_clicked()));
    connect(playlistWidget, SIGNAL(removeItemsInPL(QVector<int>)),
        this, SLOT(on_btnRemove_clicked(QVector<int>)));
    connect(playlistWidget, SIGNAL(doubleClickInPL(int)),
        this, SLOT(on_doubleClick_inPL(int)));


    // SETUP VIDEO_SLIDER ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    ui->sliderVideoPos->setMaximum(videoSliderMaxValue);    //set maximum for video position slider with global variable

                                                            //for mouse events on video slider to set position
    connect(ui->sliderVideoPos, SIGNAL(onClick(double)),
            this, SLOT(on_sliderVideoPos_clickSliderClicked(double)));
    connect(ui->sliderVideoPos, SIGNAL(onRelease()),
            this, SLOT(on_sliderVideoPos_clickSliderReleased()));
    connect(ui->sliderVideoPos, SIGNAL(onMove(double)),
            this, SLOT(on_sliderVideoPos_clickSliderMoved(double)));


    // SETUP VOLUME_SLIDER ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    ui->lblVolPercent->setText(QString::number(player->volume()) + " %");
    ui->sliderVol->setValue(player->volume());

                                                            //for mouse events on vol slider to set volume
    connect(ui->sliderVol, SIGNAL(onClick(double)),
            this, SLOT(on_sliderVolPos_clickSliderMoved(double)));
    connect(ui->sliderVol, SIGNAL(onMove(double)),
            this, SLOT(on_sliderVolPos_clickSliderMoved(double)));


    // INSTALLING EVENT FILTER FOR KEY EVENTS ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    this->installEventFilter(this);
    fullScreenWidget->installEventFilter(this);
}





void MainWindow::openFiles()
{
    QMediaPlayer::State tempPlayerState = player->state();  //save player state if the user cancel the file dialog
    player->pause();

    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Select one or more files to open", QDir::homePath(),
                                                          "Video Files (*.avi *.mpg *.mp4 *.wmv);;Audio Files (*.wav);;All files (*.*)");


    if(!filePaths.isEmpty()){                               //if the user didn't cancel the dialog, the QStringList shouldn't be empty
        setupNewPlaylistAndPlay(filePaths);
    }
    else {
        setPlayerState(tempPlayerState);                    //if the user canceled the dialog, set the player state as it was before
    }
}

void MainWindow::openDir()
{
    QMediaPlayer::State tempPlayerState = player->state();  //save player state if the user cancel the file dialog
    player->pause();

    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select a directory - all contained video files will be opened in the playlist"), QDir::homePath());
//    QString dirPath = testDir;
    QDir directory(dirPath);
    QStringList fileNames = directory.entryList(QStringList() << "*.avi" << "*.wmv" << "*.mp4" << "*.mpg", QDir::Files);
                                            //***maybe creating a menue which file types shall be included (depending on available codecs)?

    if(!fileNames.isEmpty()){                               //if the user didn't cancel the dialog, the QStringList shouldn't be empty
        for(QString & fileName: fileNames){                 //for the QMediaPlaylist add the directory path in front of the file name
            fileName = dirPath + "/" + fileName;
        }
        setupNewPlaylistAndPlay(fileNames);
    }
    else {
        setPlayerState(tempPlayerState);                    //if the user canceled the dialog, set the player state as it was before
    }
}

void MainWindow::setupNewPlaylistAndPlay(const QStringList &filePaths)
{
    this->clearPlaylist();                                  //at first clear playlist...
    addToPlaylist(filePaths);                               //...then add the file paths in the QStringList to the playlist and the playlist widget
    this->setCurrentPlaylistIndex(0);                       //...and jump to the first video or audio clip in the playlist
    if(playlist->mediaCount() > 1){                         //if there is more than one clip in the playlist...
        playlistWidget->setVisible(true);                       //...show the playlist
    }
    this->play();
}

void MainWindow::addToPlaylist()
{
    QMediaPlayer::State tempPlayerState = player->state();  //save player state and if necessary pause playing during the file dialog
    player->pause();

    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Add one or more files to playlist", QDir::homePath(),
                                                          "Video Files (*.avi *.mpg *.mp4 *.wmv);;Audio Files (*.wav);;All files (*.*)");
    addToPlaylist(filePaths);
    setPlayerState(tempPlayerState);                        //set the player state as it was before the file dialog
}

void MainWindow::addToPlaylist(const QStringList &filePaths)
{
    for(const QString & filePath: filePaths){
        if (playlist->addMedia(QUrl::fromLocalFile(filePath))){ //if file could be added to the playlist...
            QFileInfo fi(filePath);                                 //get only the filename from the path
            QString fileName = fi.fileName();
            playlistWidget->addToPlaylist(fileName);                //and add it to the visible playlist widget
        }
        else {                                                  //otherwise...
            QMessageBox::warning(this, "Adding files to playlist", "The following file couldn't be added to the playlist:\n" + filePath);
        }
    }
}

void MainWindow::moveInPlaylist(const int &selectedIndex, const int &direction)
{
    //***not perfect solution, because the video has to be new buffered when changing position in the playlist  :(
    //(note: "if(selectedIndex == 0 || selectedIndex == playlist->mediaCount())" (resp. borders of moving) is already checked in "PlaylistWIdget.cpp"

    int currentPlaylistIndex = playlist->currentIndex();            //save current playlist index, player position and player state before...
    qint64 tempPlayerPosition = player->position();
    QMediaPlayer::State tempPlayerState = player->state();
    player->pause();                                                //...pausing the player and...
    playlist->moveMedia(selectedIndex, selectedIndex + direction);  //...moving media (at the in the playlist widget selected index) in the playlist

    if(currentPlaylistIndex == selectedIndex){                      //if the selected index is the actually the current playing index...
        playlist->setCurrentIndex(selectedIndex + direction);           //...jump to the new index position...
        player->setPosition(tempPlayerPosition);                        //...and bring back the player position that it had before
    }
    else if (currentPlaylistIndex == (selectedIndex + direction)){  //if the new index position is equal the current playing index...
        playlist->setCurrentIndex(selectedIndex);                       //...jump to the "old index", to where the current playing clip has been moved
        player->setPosition(tempPlayerPosition);                        //...and bring back the player position that it had before
    }
    else {                                                          //otherwise...
        playlist->setCurrentIndex(currentPlaylistIndex);                //...just set the current playlist index where it was before (required after "moveMedia"!)
        setCurrentPlaylistIndex(selectedIndex + direction);             //...but the selection in the playlist widget shall stay the moved index
    }
    player->setPosition(tempPlayerPosition);                        //set player position and state as it was before
    setPlayerState(tempPlayerState);
}

void MainWindow::setCurrentPlaylistIndex(const int &index)
{
    playlistWidget->setCurrentIndex(index);
}

void MainWindow::clearPlaylist()
{
    playlist->clear();
    playlistWidget->clearPlaylist();
}

void MainWindow::setPlayerState(const QMediaPlayer::State &state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        player->stop();
        break;
    case QMediaPlayer::PlayingState:
        this->play();
        break;
    case QMediaPlayer::PausedState:
        player->pause();
        break;
    }
}

void MainWindow::play()
{
    if(quizNewClip){        //***for quiz mode
        srand((unsigned)time(NULL));                                    //two random functions for more distribution...?
        int randomA = rand() % 100;                                     //***improvable!
        std::uniform_int_distribution<int> randDistribution(0,100);
        int randomB = randDistribution(*QRandomGenerator::global());
        int randomC = (randomA * randomB) % 100;

        quizVideoStartPos = static_cast<qint64>(videoDuration * (randomC / 100.0)); //calculate random start position
        player->setPosition(quizVideoStartPos);                         //set the player at the random start position
    }

    player->play();
}

bool MainWindow::eventFilter(QObject *sender, QEvent *event)        //event filter for key events...
{
    if(event-> type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Space){                      //space = play / pause
            on_btnPlayPause_clicked();
        }
        if (keyEvent->key() == Qt::Key_B){                          //b = previus (or "back") clip in playlist
            on_btnPrevious_clicked();
        }
        if (keyEvent->key() == Qt::Key_N){                          //n = next clip in playlist
            on_btnNext_clicked();
        }
        if (keyEvent->key() == Qt::Key_F){                          //f = fullscreen (on / off)
            if(fullScreenWidget->isVisible()){
                on_fullScreen_doubleClick();
            }
            else{
                on_btnFullscreen_clicked();
            }
        }
        if (keyEvent->key() == Qt::Key_Escape){                     //Esc = Escape from Fullscreen
            if(fullScreenWidget->isVisible()){
                on_fullScreen_doubleClick();
            }
        }
    }
    //Event weiter durchreichen an die ursprüngliche methode von QObject
    return QObject::eventFilter(sender, event);

}


