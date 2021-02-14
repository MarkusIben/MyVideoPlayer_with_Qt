#ifndef CLICKABLESLIDER_H
#define CLICKABLESLIDER_H

#include <QObject>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>


class ClickableSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ClickableSlider(QWidget *parent = nullptr);


signals:
    void onClick(double mouseXPosOnSliderInPercent);
    void onMove(double mouseXPosOnSliderInPercent);
    void onRelease();

protected:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    bool leftMouseButtonIsPressed = false;

    double caculateMouseXPosOnSliderInPercent(const int &MouseEventXPos);

};

#endif // CLICKABLESLIDER_H
