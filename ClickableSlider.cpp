#include "ClickableSlider.h"

ClickableSlider::ClickableSlider(QWidget *parent) :
    QSlider(parent)
{

}

void ClickableSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){                         //When pressing with left button on the slider...
        leftMouseButtonIsPressed = true;                                //set the member variable on true (so that the move will be proceed!)
        emit onClick(caculateMouseXPosOnSliderInPercent(event->x()));   //and send the percentage x Pos of the mouse on the slider to MainWindow
    }                                                                   //(...for changing player position)
}

void ClickableSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){                         //see "mousePressEvent" and "mouseMoveEvent"
        leftMouseButtonIsPressed = false;
        emit onRelease();
    }
}

void ClickableSlider::mouseMoveEvent(QMouseEvent *event)
{
    if (leftMouseButtonIsPressed){                                  //While the mouse is pressed...
        emit onMove(caculateMouseXPosOnSliderInPercent(event->x()));    //and send the percentage x Pos of the mouse on the slider to MainWindow
    }                                                                   //(...for changing player position)
}

double ClickableSlider::caculateMouseXPosOnSliderInPercent(const int &MouseEventXPos)
{
    double mouseXPosOnSlider = static_cast<double>(MouseEventXPos);
    double sliderWidth = static_cast<double>(this->width());
    double mouseXPosOnSliderInPercent = mouseXPosOnSlider / sliderWidth;
    if (mouseXPosOnSliderInPercent < 0){                            //when moving the curser (after pressed on the slieder) outside of the slider...
        mouseXPosOnSliderInPercent = 0;                             //...the value "mouseXPosOnSliderInPercent" can become negative...
    }                                                               //...or greater then 1, but as a value in percent is has to be between 0 and 1
    else if (mouseXPosOnSliderInPercent > 1){
        mouseXPosOnSliderInPercent = 1;
    }
    return mouseXPosOnSliderInPercent;

}
