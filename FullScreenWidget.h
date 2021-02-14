#ifndef FULLSCREENWIDGET_H
#define FULLSCREENWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QBoxLayout>

namespace Ui {
class FullScreenWidget;
}

class FullScreenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenWidget(QWidget *parent = nullptr);
    ~FullScreenWidget() override;

    void addWidgetToLayout(QWidget* widget);

signals:
    void on_doubleClick();
    void on_mousePress();


protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::FullScreenWidget *ui;
    QHBoxLayout* fullScreenLayout = nullptr;
};

#endif // FULLSCREENWIDGET_H
