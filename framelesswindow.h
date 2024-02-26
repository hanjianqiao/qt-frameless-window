#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace h {

class FramelessWindowTitleBar : public QWidget
{
    Q_OBJECT
public:
    FramelessWindowTitleBar(QWidget *parent);

    virtual void onWindowStateChanged(Qt::WindowState state) = 0;
    virtual QWidget *maximizeButton() const = 0;
    virtual void setTitle(const QString &title);
    virtual void setIcon(const QIcon &icon);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;
};

class FramelessWindowDefaultTitleBar : public FramelessWindowTitleBar
{
    Q_OBJECT
public:
    FramelessWindowDefaultTitleBar(QWidget *parent);

    void onWindowStateChanged(Qt::WindowState state) override;
    QWidget *maximizeButton() const override;
    void setTitle(const QString &title) override;
    void setIcon(const QIcon &icon) override;

private:
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeButton;
    QPushButton *m_closeButton;
};

class FramelessWindow : public QWidget
{
    Q_OBJECT

public:
    FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();

    void setTitleBarAndBodyWidget(FramelessWindowTitleBar *titleBar, QWidget *bodyWidget);
    FramelessWindowTitleBar *titleBar() const;

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    void changeEvent(QEvent *event) override;

    // make title draggable
    bool eventFilter(QObject *object, QEvent *event) override;

    // config the window to be frameless
    void configFrameless();

protected:
    FramelessWindowTitleBar *m_titleBar;
    QWidget *m_bodyWidget;
};
}
#endif // FRAMELESSWINDOW_H
