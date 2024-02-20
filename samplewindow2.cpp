#include <QVBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QProgressBar>
#include "samplewindow2.h"

CustomizedTitleBarContentWidget::CustomizedTitleBarContentWidget(QWidget *parent)
    : QWidget{parent}
{
    QLabel *label = new QLabel("这是一个Label", this);
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText("输入搜索");
    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0);
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems({"第一个", "第二个", "第三个", });
    QRadioButton *radioButton = new QRadioButton("QRadioButton", this);

    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(label);
    rootLayout->addWidget(lineEdit);
    rootLayout->addWidget(progressBar);
    rootLayout->addWidget(comboBox);
    rootLayout->addWidget(radioButton);
}

CustomizedTitleBarControlWidget::CustomizedTitleBarControlWidget(QWidget *parent)
    : QWidget{parent}
{
    m_settingButton = new QPushButton(this);
    m_settingButton->setIcon(QIcon(":/resources/robot.jpg"));
    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    m_maximizeButton = new h::FramelessWindowMaximizeButton(this);
    m_maximizeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    m_closeButton = new QPushButton(this);
    m_closeButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(m_settingButton);
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

h::FramelessWindowMaximizeButton *CustomizedTitleBarControlWidget::maximizeButton() const{
    return m_maximizeButton;
}

SampleWindow2::SampleWindow2()
    : FramelessWindow{}
{
    CustomizedTitleBarControlWidget *titleBarControlWidget = new CustomizedTitleBarControlWidget(this);
    h::FramelessWindowTitleBar *titleBar = new h::FramelessWindowTitleBar(
        new CustomizedTitleBarContentWidget(this),
        titleBarControlWidget,
        titleBarControlWidget->maximizeButton()
        );
    setTitleBar(titleBar);
    setMaximizeButton(titleBarControlWidget->maximizeButton());


    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(30, 20, 50, 200));
    titleBar->setAutoFillBackground(true);
    titleBar->setPalette(pal);

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->addWidget(titleBar);
    rootLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Expanding));
    rootLayout->setContentsMargins(0, 0, 0, 0);
}
