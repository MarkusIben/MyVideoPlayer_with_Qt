#include "ProgressSlider.h"

ProgressSlider::ProgressSlider(QWidget *parent) :
    QSlider(parent)
{

}

void ProgressSlider::mousePressEvent(QMouseEvent *event)
{
    QStyleOptionSlider opt;
      initStyleOption(&opt);
      QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

      if (event->button() == Qt::LeftButton && !sr.contains(event->pos())) {
        int newVal;
        if (orientation() == Qt::Vertical) {
           double halfHandleHeight = (0.5 * sr.height()) + 0.5;
           int adaptedPosY = height() - event->y();
           if ( adaptedPosY < halfHandleHeight )
                 adaptedPosY = halfHandleHeight;
           if ( adaptedPosY > height() - halfHandleHeight )
                 adaptedPosY = height() - halfHandleHeight;
           double newHeight = (height() - halfHandleHeight) - halfHandleHeight;
           double normalizedPosition = (adaptedPosY - halfHandleHeight)  / newHeight ;

           newVal = minimum() + (maximum()-minimum()) * normalizedPosition;
        } else {
            double halfHandleWidth = (0.5 * sr.width()) + 0.5;
            int adaptedPosX = event->x();
            if ( adaptedPosX < halfHandleWidth )
                  adaptedPosX = halfHandleWidth;
            if ( adaptedPosX > width() - halfHandleWidth )
                  adaptedPosX = width() - halfHandleWidth;
            double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
            double normalizedPosition = (adaptedPosX - halfHandleWidth)  / newWidth ;

            newVal = minimum() + qRound((maximum()-minimum()) * normalizedPosition);
        }

        if (invertedAppearance()){
            this->setValue( maximum() - newVal );
        }
        else{
            this->setValue(newVal);
        }

        event->accept();
      }
      else {
            QSlider::mousePressEvent(event);
      }
      emit onClick(this->value());
}
