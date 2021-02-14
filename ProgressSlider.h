#ifndef PROGRESSSLIDER_H
#define PROGRESSSLIDER_H

#include <QObject>
#include <QWidget>
#include <QAbstractSlider>
#include <QSlider>
#include <QMouseEvent>
#include <QStyle>
#include <QStyleOptionSlider>

class ProgressSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ProgressSlider(QWidget *parent = nullptr);

signals:
    void onClick(int value);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // PROGRESSSLIDER_H
