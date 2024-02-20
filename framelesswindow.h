#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace h {

class FramelessWindowMaximizeButton : public QPushButton
{
    Q_OBJECT
public:
    FramelessWindowMaximizeButton(QWidget *parent = nullptr);
    FramelessWindowMaximizeButton(const QString &title, QWidget *parent = nullptr);

protected:
    bool event(QEvent *e) override;

};

class FramelessWindowTitleBarContentWidget : public QWidget
{
    Q_OBJECT
public:
    FramelessWindowTitleBarContentWidget(const QString &title, const QIcon &icon, QWidget *parent);

protected:
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
};

class FramelessWindowTitleBarControlWidget : public QWidget
{
    Q_OBJECT
public:
    FramelessWindowTitleBarControlWidget(QWidget *parent);
    FramelessWindowMaximizeButton *maximizeButton() const;

private:
    QPushButton *m_minimizeButton;
    FramelessWindowMaximizeButton *m_maximizeButton;
    QPushButton *m_closeButton;
};

class FramelessWindowTitleBar : public QWidget
{
    Q_OBJECT
public:
    FramelessWindowTitleBar(QWidget *contentWidget, QWidget *controlWidget, QPushButton *maximizeButton, QWidget *parent = nullptr);
    void onWindowStateChanged(Qt::WindowState state);
    QPushButton *maximizeButton() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

protected:
    QPushButton *m_maximizeButton;

};

class FramelessWindow : public QWidget
{
    Q_OBJECT

public:
    FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();

    void setTitleBar(FramelessWindowTitleBar *titleBar);
    void setMaximizeButton(QPushButton *button);

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    void changeEvent(QEvent *event) override;

    // make title draggable
    bool eventFilter(QObject *object, QEvent *event) override;

    // config the window to be frameless
    void configFrameless();

protected:
    FramelessWindowTitleBar *m_titleBar;
    QPushButton *m_maximizeButton;

};
}
#endif // FRAMELESSWINDOW_H
