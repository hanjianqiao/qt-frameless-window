#ifndef SAMPLEWINDOW2_H
#define SAMPLEWINDOW2_H

#include "framelesswindow.h"

class CustomizedTitleBarContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomizedTitleBarContentWidget(QWidget *parent);

};

class CustomizedTitleBarControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomizedTitleBarControlWidget(QWidget *parent);
    h::FramelessWindowMaximizeButton *maximizeButton() const;

private:
    QPushButton *m_settingButton;
    QPushButton *m_minimizeButton;
    h::FramelessWindowMaximizeButton *m_maximizeButton;
    QPushButton *m_closeButton;
};

class SampleWindow2 : public h::FramelessWindow
{
    Q_OBJECT
public:
    explicit SampleWindow2();

signals:
};

#endif // SAMPLEWINDOW2_H
