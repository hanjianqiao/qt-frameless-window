#include <QEvent>
#include <QWindow>
#include <QScreen>
#include <QMouseEvent>
#include <QApplication>
#include <QStyle>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QTimer>

#include "framelesswindow.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#pragma comment(lib, "dwmapi")
#pragma comment(lib, "user32.lib")
#include <dwmapi.h>
#include <windowsx.h>
#endif

namespace h {
FramelessWindowTitleBar::FramelessWindowTitleBar(QWidget *parent)
    : QWidget{parent}
{
}

void FramelessWindowTitleBar::mousePressEvent(QMouseEvent *event){
    window()->windowHandle()->startSystemMove();
    QWidget::mousePressEvent(event);
}

void FramelessWindowTitleBar::mouseDoubleClickEvent(QMouseEvent *event){
    if(window()->isMaximized()){
        window()->showNormal();
    }else{
        window()->showMaximized();
    }
}

bool FramelessWindowTitleBar::eventFilter(QObject *obj, QEvent *event){
    if (!obj->isWidgetType()) return false; // standard check
    if(obj == maximizeButton()){
        switch (event->type()) {
        case QEvent::HoverMove:
            maximizeButton()->setAttribute(Qt::WidgetAttribute::WA_UnderMouse);
            break;
        case QEvent::HoverLeave:
            maximizeButton()->setAttribute(Qt::WidgetAttribute::WA_UnderMouse, false);
            break;
        default:
            break;
        }
    }
    return false;
}

FramelessWindowDefaultTitleBar::FramelessWindowDefaultTitleBar(QWidget *parent)
    : FramelessWindowTitleBar{parent}
{
    m_iconLabel = new QLabel(this);
    m_iconLabel->setPixmap(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(16, 16));
    m_titleLabel = new QLabel(qApp->applicationName(), this);
    m_minimizeButton = new QPushButton(this);
    m_maximizeButton = new QPushButton(this);
    m_closeButton = new QPushButton(this);

    m_minimizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    if(windowState() == Qt::WindowMaximized){
        m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    }else{
        m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    }
    m_closeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    /*
     * BUG：FramelessWindow通过nativeEvent实现鼠标悬浮在最大化按钮触发snap layout的功能
     * 时，发送QEvent::MouseMove事件，但是目标收到的是QEvent::HoverMove事件，无法触发
     * QPushButton的状态变化。暂时通过这个代码处理事件改变QPushButton的状态。QPushButton
     * 也可以是普通的QWidget
    */
    m_maximizeButton->installEventFilter(this);

    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(8, 0, 0, 0);
    rootLayout->addWidget(m_iconLabel);
    rootLayout->addWidget(m_titleLabel);
    rootLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Preferred));
    rootLayout->addWidget(m_minimizeButton);
    rootLayout->addWidget(m_maximizeButton);
    rootLayout->addWidget(m_closeButton);

    QObject::connect(m_minimizeButton, &QPushButton::clicked, this, [this](){
        window()->showMinimized();
    });
    QObject::connect(m_closeButton, &QPushButton::clicked, this, [this](){
        window()->close();
    });
}

void FramelessWindowDefaultTitleBar::onWindowStateChanged(Qt::WindowState state){
    switch(state){
    case Qt::WindowState::WindowNoState:
        m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        break;
    case Qt::WindowState::WindowMaximized:
        m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        break;
    default:
        break;
    }
}

QWidget *FramelessWindowDefaultTitleBar::maximizeButton() const{
    return m_maximizeButton;
}

void FramelessWindowDefaultTitleBar::setTitle(const QString &title){
    m_titleLabel->setText(title);
}

void FramelessWindowDefaultTitleBar::setIcon(const QIcon &icon){
    m_iconLabel->setPixmap(icon.pixmap(16, 16));
}

FramelessWindow::FramelessWindow(QWidget *parent)
    : QWidget(parent), m_titleBar{nullptr}, m_bodyWidget{nullptr}
{
    configFrameless();
}

FramelessWindow::~FramelessWindow() {

}

void FramelessWindow::setTitleBarAndBodyWidget(FramelessWindowTitleBar *titleBar, QWidget *bodyWidget){
    m_titleBar = titleBar;
    m_bodyWidget = bodyWidget;

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    m_bodyWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rootLayout->addWidget(m_titleBar);
    rootLayout->addWidget(m_bodyWidget);
}

FramelessWindowTitleBar *FramelessWindow::titleBar() const{
    return m_titleBar;
}

void FramelessWindow::configFrameless() {
    setWindowFlags(Qt::Window
                   | Qt::WindowMaximizeButtonHint
                   | Qt::WindowMinimizeButtonHint
                   | Qt::FramelessWindowHint);

    HWND hWnd = (HWND)winId();
    LONG style = GetWindowLongW(hWnd, GWL_STYLE);
    SetWindowLong(
        hWnd,
        GWL_STYLE,
        style
            | WS_MINIMIZEBOX
            | WS_MAXIMIZEBOX
            | WS_CAPTION
            | CS_DBLCLKS
            | WS_THICKFRAME
        );
}
bool FramelessWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
    const static int borderThickness = 5;
    MSG* pMsg = reinterpret_cast<MSG*>(message);
    switch (pMsg->message)
    {
    //无边框窗口
    case WM_NCCALCSIZE:
    {
        *result = 0;
        return true;
        break;
    }
    //阴影、圆角
    case WM_ACTIVATE:
    {
        MARGINS margins = { 1, 1, 0, 1 };
        HRESULT hr = S_OK;
        hr = DwmExtendFrameIntoClientArea(pMsg->hwnd, &margins);
        *result = hr;
        return true;
    }
    // 触发贴边停靠的条件：1、窗口样式（默认的样式即可）。2、鼠标靠近边缘
    case WM_NCHITTEST:
    {
        //处理resize
        //标记只处理resize
        bool isResize = false;
        // Get the point coordinates for the hit test.
        POINT ptMouse = { GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)};
        // Get the window rectangle.
        RECT rcWindow;
        GetWindowRect(pMsg->hwnd, &rcWindow);
        // Get the frame rectangle, adjusted for the style without a caption.
        RECT rcFrame = {0};
        AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);
        // Determine if the hit test is for resizing. Default middle (1,1).
        USHORT uRow = 1;
        USHORT uCol = 1;
        bool fOnResizeBorder = false;

        // Determine if the point is at the top or bottom of the window.
        if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + borderThickness)
        {
            fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
            uRow = 0;
            isResize = true;
        }
        else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - borderThickness)
        {
            uRow = 2;
            isResize = true;
        }
        // Determine if the point is at the left or right of the window.
        if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + borderThickness)
        {
            uCol = 0; // left side
            isResize = true;
        }
        else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - borderThickness)
        {
            uCol = 2; // right side
            isResize = true;
        }
        LRESULT hitTests[3][3] =
            {
             { HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
             { HTLEFT,       HTNOWHERE,     HTRIGHT },
             { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
             };

        if (isResize == true)
        {
            *result = hitTests[uRow][uCol];
            return true;
        }

        if(m_titleBar && m_titleBar->maximizeButton()){
            //support highdpi/multiple screen
            QPoint gpos(ptMouse.x, ptMouse.y);
            QWindow * handle = window()->windowHandle();
            if(handle && handle->screen())
            {
                QScreen * screen = handle->screen();
                QPoint offset = screen->geometry().topLeft();
                gpos = (gpos - offset) / screen->devicePixelRatio() + offset;
            }
            QPoint lpos = m_titleBar->maximizeButton()->mapFromGlobal(gpos);
            // TODO: return true; 会让Qt获取不到事件，发送对应事件并更新，使maximizeButton响应鼠标移动
            if (m_titleBar->maximizeButton()->rect().contains(lpos)){
                if(!m_titleBar->maximizeButton()->testAttribute(Qt::WA_UnderMouse)){
                    QMouseEvent* evt = new QMouseEvent(
                        QEvent::HoverMove,
                        lpos,
                        gpos,
                        Qt::LeftButton,
                        QGuiApplication::mouseButtons(),
                        QGuiApplication::keyboardModifiers()
                        );
                    QCoreApplication::sendEvent(m_titleBar->maximizeButton(), evt);
                    m_titleBar->maximizeButton()->update();
                }
                *result = HTMAXBUTTON;
                return true;
            }else{
                if(m_titleBar->maximizeButton()->testAttribute(Qt::WA_UnderMouse)){
                    QMouseEvent* evt = new QMouseEvent(
                        QEvent::HoverLeave,
                        lpos,
                        gpos,
                        Qt::LeftButton,
                        QGuiApplication::mouseButtons(),
                        QGuiApplication::keyboardModifiers()
                        );
                    QCoreApplication::sendEvent(m_titleBar->maximizeButton(), evt);
                    m_titleBar->maximizeButton()->update();
                    return true;
                }
            }
        }

        // 在客户区
        *result = HTCLIENT;
        return false;
    }
    // snap layout 功能使最大化按钮无法接收到鼠标事件
    case WM_NCLBUTTONDOWN:
        if(pMsg->wParam == HTMAXBUTTON)
        {
            *result = 0;
            return true;
        }
    case WM_NCLBUTTONUP:
        if(pMsg->wParam == HTMAXBUTTON)
        {
            *result = 0;
            if(isMaximized()){
                qDebug() << "showNormal";
                showNormal();
            }else{
                qDebug() << "showMaximized";
                showMaximized();
            }
            return true;
        }
    case WM_NCLBUTTONDBLCLK:
        if(pMsg->wParam == HTMAXBUTTON)
        {
            *result = 0;
            return true;
        }
        break;
    case WM_NCMOUSEHOVER:
    case WM_NCMOUSELEAVE:
    case WM_NCMOUSEMOVE:
        if(pMsg->wParam == HTMAXBUTTON)
        {
            *result = 0;
            // 暂时不处理
            return true;
        }
    default:
        return false;
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void FramelessWindow::changeEvent(QEvent *event){
    switch(event->type()){
    case QEvent::WindowStateChange:
        if(windowState() == Qt::WindowMaximized){
            m_titleBar->onWindowStateChanged(Qt::WindowState::WindowMaximized);
            QTimer::singleShot(100, this, [this](){
                int borderThickness = GetSystemMetrics(SM_CXSIZEFRAME);
                setContentsMargins(borderThickness, borderThickness, borderThickness, borderThickness);
            });
        }else if(windowState() == Qt::WindowState::WindowNoState){
            m_titleBar->onWindowStateChanged(Qt::WindowState::WindowNoState);
            setContentsMargins(0, 0, 0, 0);
        }
        break;
    default:
        break;
    }
    return QWidget::changeEvent(event);
}

bool FramelessWindow::eventFilter(QObject *obj, QEvent *event){
    if (!obj->isWidgetType()) return false; // standard check
    auto w = static_cast<QWidget*>(obj);
    if(w == m_titleBar && event->type() == QEvent::MouseButtonPress){
        window()->windowHandle()->startSystemMove();
    }
    return false;
}

}
