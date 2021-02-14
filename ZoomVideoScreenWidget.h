#ifndef ZOOMVIDEOSCREENWIDGET_H
#define ZOOMVIDEOSCREENWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QBoxLayout>
#include <QDebug>
#include <QWheelEvent>
#include <QCursor>
#include <QApplication>

class ZoomVideoScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomVideoScreenWidget(QWidget *parent = nullptr);

    QVideoWidget* getVideoScreen();
    void resetZoom();

signals:

public slots:

protected:
    void wheelEvent(QWheelEvent *event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QVideoWidget* videoScreen;
    QHBoxLayout* zoomVideoScreenLayout;
};

#endif // ZOOMVIDEOSCREENWIDGET_H
