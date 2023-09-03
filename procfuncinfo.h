
#ifndef PROC_FUNC_INFO_H_H
#define PROC_FUNC_INFO_H_H
#include <QObject>
#include <QDateTime>
#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <QThread>
#include <string>
#include <QSettings>
#include <QVector>
#include <stdint.h>
#include <QFileDialog>  //头文件
#include <QFileDialog>
#include <QApplication>
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QStringList>
#include <QModelIndex>
#include <QDir>
#include <QDebug>
#include <QObject>
#include <QFileDialog>
using namespace std;

class CFunction;

class SchedulingStrategy
{
public:
    SchedulingStrategy(CFunction *func);
    int GetProxyInfo( QVector <QString> &needcapdata, QVector <QString> &vecStr, QString &res);
    CFunction *m_func;
};

#endif
