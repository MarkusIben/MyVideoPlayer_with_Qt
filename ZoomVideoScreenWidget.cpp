#include "ZoomVideoScreenWidget.h"

ZoomVideoScreenWidget::ZoomVideoScreenWidget(QWidget *parent) : QWidget(parent)
{
    videoScreen = new QVideoWidget(this);                   //initialize a QVideoWidget ("videoScreen") and a Layout and...
    zoomVideoScreenLayout = new QHBoxLayout();
    zoomVideoScreenLayout->setMargin(0);
    this->setLayout(zoomVideoScreenLayout);                 //...set the layout to this "ZoomVideoScreenWidget" object...
    zoomVideoScreenLayout->addWidget(videoScreen);          //...and add the videoScreen to it

    QPalette tempPalette = this->palette();
    tempPalette.setColor(QPalette::Background, Qt::black);  //normally should change video screen widget background color to black
    this->setPalette(tempPalette);                          //*** doesn't work like wanted! why?
}

QVideoWidget *ZoomVideoScreenWidget::getVideoScreen()
{
    return this->videoScreen;                               //implemented getter so MainWindow can pair QMediaPlayer and QVideowidget
}

void ZoomVideoScreenWidget::resetZoom()                     //When changing media in playlist reset the zoom
{
    this->videoScreen->setGeometry(this->x(),this->y(),this->width(),this->height());
}

void ZoomVideoScreenWidget::wheelEvent(QWheelEvent *event)
{
    int wheelYRotation = event->angleDelta().y();                           // +/- 120 per wheel step
    int zoomStep = wheelYRotation / 3;                                      // define that one zoom step means 40 pixels (120 / 3)

    QPoint cursorPos = QCursor::pos();                                      //get cursor position and...
    QPoint cursorPosRelToWidget = this->mapFromGlobal(cursorPos);           //...map it to the zoom video screen widget

    double widgetWidth = static_cast<double>(this->width());                //get widget with and heigth as double for calculation...
    double widgetHeight = static_cast<double>(this->height());
    double cursorXPosInPercent = cursorPosRelToWidget.x() / widgetWidth;    //calculate x and y position of the cursor on the widget in %
    double cursorYPosInPercent = cursorPosRelToWidget.y() / widgetHeight;

    int zoomWidth = videoScreen->width() + zoomStep;                  //the width and height of the video screen changes...
    int zoomHeight = videoScreen->height() + zoomStep;                //...(with 2 * zoomstep)

    int zoomPosX = videoScreen->pos().x();                                  //get the actual origin of the videoScreen...
    int zoomPosY = videoScreen->pos().y();                                  //...which will be outside of the widget after zooming in

    if (zoomWidth < this->width()){                                         //the video screen shall never be smaller...
        zoomPosX = 0;                                                       //...than the video screen widget!...
        zoomWidth = this->width();
    }
    else{                                                                   //...but besides that, the video screen origin will be moved...
        zoomPosX -= static_cast<int>(cursorXPosInPercent * zoomStep);       //...according to the cursor position in percent
    }
    if (zoomHeight < this->height()){
        zoomPosY = 0;
        zoomHeight = this->height();
    }
    else{
        zoomPosY -= static_cast<int>(cursorYPosInPercent * zoomStep);
    }

    //While zooming in and out with moving the video screens origin, all the time the whole video screen widget...
    //...shall be used for displaying, so the origin of the video screen get borders

    if (zoomPosX < (this->width() - zoomWidth)) {
        zoomPosX = (this->width() - zoomWidth);
    }
    else if (zoomPosX > 0){
        zoomPosX = 0;
    }
    if (zoomPosY < (this->height() - zoomHeight)){
        zoomPosY = (this->height() - zoomHeight);
    }
    else if (zoomPosY > 0){
        zoomPosY = 0;
    }

    //***the Qt method "setGeometry" causes a jerky zoom, because u can perceive the time gap between scaling (first) and moving (second)
    this->videoScreen->setGeometry(zoomPosX,zoomPosY,zoomWidth,zoomHeight);
}

void ZoomVideoScreenWidget::enterEvent(QEvent *)                    //Show other cursor when mouse is entering the video widget...
{                                                                   //..as a hint, that the user can zoom in and out!
    QApplication::setOverrideCursor(Qt::PointingHandCursor);        //(magnifying glass would be better than the pointing hand...
}                                                                   //...but Qt doesn't offer it)

void ZoomVideoScreenWidget::leaveEvent(QEvent *)                    //When mouse is leaving the video widget restore old cursor
{
    QApplication::restoreOverrideCursor();
}
