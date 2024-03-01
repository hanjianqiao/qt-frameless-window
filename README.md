# qt-frameless-window 简单易用的Qt无边框窗体库

> 运行使用示例请移步[qt-frameless-window-sample](https://github.com/hanjianqiao/qt-frameless-window-sample)

## 说明
1. 目前仅支持Windows
2. 支持特性：Windows Snap（拖拽快速布局）、拖拽缩放
3. 支持标题栏完全自定义

## 使用方式

1. 在项目主路径下执行命令
   ```bash
   git submodule add https://github.com/hanjianqiao/qt-frameless-window.git submodules/qt-frameless-window
   ```
2. 集成到你的项目
  
   2.1. 修改CMakeLists.txt，添加下面指令
   ```
   # 假设你的目标名称为：yourTarget
   add_subdirectory(submodules/qt-frameless-window)
   target_include_directories(yourTarget PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/submodules")
   target_link_libraries(yourTarget PRIVATE qt-frameless-window)
   ```
   2.2 修改Project.pro，添加下面的代码即可
   ```
   include(submodules/qt-frameless-window/qt-frameless-window.pri)
   ```

3. 继承`h::FramelessWindow`，进行少量配置即可，参考代码如下
    ```c++
    #include <QApplication>
    #include <QStyle>
    #include "samplewindow.h"
    using namespace h;
    SampleWindow::SampleWindow() : FramelessWindow{}
    {
        // 自带的titleBar
        FramelessWindowDefaultTitleBar *defaultTitleBar = new FramelessWindowDefaultTitleBar(this);
        // 可以是任意的Widget
        QWidget *bodyWidget = new QLabel("ABC", this);
        setTitleBarAndBodyWidget(defaultTitleBar, bodyWidget);
        // 标题跟图标
        titleBar()->setTitle("这是标题");
        titleBar()->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    }
    ```
    
### 效果1：基本无边框窗口
![基本无边框窗口](https://github.com/hanjianqiao/qt-frameless-window/assets/7146341/bc1e261e-aaa8-40c8-b522-ad846c9969fa)
### 效果2：自定义标题栏
![自定义标题栏](https://github.com/hanjianqiao/qt-frameless-window/assets/7146341/0094c65f-206f-409e-b5b8-4b02dfff79b8)

## 更多
本项目使用MIT协议，欢迎使用、提建议、参与贡献。
