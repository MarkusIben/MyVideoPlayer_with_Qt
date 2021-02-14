#include "FullScreenWidget.h"
#include "ui_FullScreenWidget.h"

FullScreenWidget::FullScreenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullScreenWidget)
{
    ui->setupUi(this);

    fullScreenLayout = new QHBoxLayout;
    fullScreenLayout->setMargin(0);
    this->setLayout(fullScreenLayout);
}

FullScreenWidget::~FullScreenWidget()
{
    delete ui;
}

void FullScreenWidget::addWidgetToLayout(QWidget *widget)
{
    fullScreenLayout->addWidget(widget);
}

void FullScreenWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    emit on_doubleClick();
}

void FullScreenWidget::mousePressEvent(QMouseEvent *)
{
    emit on_mousePress();
}

void FullScreenWidget::mouseMoveEvent(QMouseEvent *event)
{

}
