// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <QApplication>
#include <QLabel>
#include <QDir>
#include <QSettings>
#include <qscreen>
#include "qtservice.h"

class InteractiveService : public QtService<QApplication>
{
public:
    InteractiveService(int argc, char **argv);
    ~InteractiveService();

protected:

    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);

private:
    QLabel *gui;
};

InteractiveService::InteractiveService(int argc, char **argv)
    : QtService<QApplication>(argc, argv, "Qt Interactive Service"), gui(0)
{
    setServiceDescription("A Qt service with user interface.");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

InteractiveService::~InteractiveService()
{
}

void InteractiveService::start()
{
    qApp->setQuitOnLastWindowClosed(false);

    gui = new QLabel("Service", 0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    gui->move(QGuiApplication::primaryScreen()->availableGeometry().topLeft());
    gui->show();
}

void InteractiveService::stop()
{
    delete gui;
}

void InteractiveService::pause()
{
    if (gui)
	gui->hide();
}

void InteractiveService::resume()
{
    if (gui)
	gui->show();
}

void InteractiveService::processCommand(int code)
{
    gui->setText("Command code " + QString::number(code));
    gui->adjustSize();
}

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    InteractiveService service(argc, argv);
    return service.exec();
}
