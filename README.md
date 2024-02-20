# qt-frameless-window 简单易用的Qt无边框窗体库

## 说明
1. 目前仅支持Windows
2. 支持特性：Windows Snap（拖拽快速布局）、拖拽缩放
3. 支持标题栏完全自定义

## 使用方式
1. 将本项目添加到你的项目，注意配置引用路径
2. 继承`h::FramelessWindow`，添加少量配置代码即可实现基本的无边框窗口。
3. 示例代码，可下载源码直接运行
```c++
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
```
### 效果1：基本无边框窗口
![基本无边框窗口](https://github.com/hanjianqiao/qt-frameless-window/assets/7146341/bc1e261e-aaa8-40c8-b522-ad846c9969fa)
### 效果2：自定义标题栏
![自定义标题栏](https://github.com/hanjianqiao/qt-frameless-window/assets/7146341/0094c65f-206f-409e-b5b8-4b02dfff79b8)

## 更多
本项目使用MIT协议，欢迎使用、提建议、参与贡献。
