#include <QApplication>
#include <QStyle>
#include "samplewindow.h"

using namespace h;

SampleWindow::SampleWindow()
    : FramelessWindow{}
{
    // 设置标题栏，配置最大化按钮（最大化按钮会触发Snap效果、在最大化/最小化状态变化时会改变图标）
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

    // 主窗口
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->addWidget(titleBar);    // 自定义标题栏，要求标题栏也是widget的一部分，所以需要添加到窗体布局合适位置
    rootLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Expanding));
    rootLayout->setContentsMargins(0, 0, 0, 0);
}
