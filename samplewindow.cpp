#include <QApplication>
#include <QStyle>
#include "samplewindow.h"

using namespace h;

SampleWindow::SampleWindow()
    : FramelessWindow{}
{
    FramelessWindowTitleBarControlWidget *titleBarControlWidget = new FramelessWindowTitleBarControlWidget(this);
    FramelessWindowTitleBar *titleBar = new FramelessWindowTitleBar(
        new FramelessWindowTitleBarContentWidget(
            "窗口标题",
            QApplication::style()->standardIcon(QStyle::SP_ComputerIcon),
            this),
        titleBarControlWidget,
        titleBarControlWidget->maximizeButton()
        );
    setTitleBar(titleBar);
    setMaximizeButton(titleBarControlWidget->maximizeButton());

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->addWidget(titleBar);
    rootLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Expanding));
    rootLayout->setContentsMargins(0, 0, 0, 0);
}
