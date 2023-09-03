
#include "function.h"
#include <QtWidgets/QTableWidget>
#include <QtGui>
#include <stdio.h>
#include <time.h>
#include <QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include "xdb_searcher.h"
#include <QClipboard>
#include <QApplication>
#include <QImage>
#include <QMimeData>
#include <QUrl>
#include <QByteArray>
#include <Windows.h>
#include <tlhelp32.h>
#include "pcap.h"
#include<QListView>
#include <psapi.h> // For access to GetModuleFileNameEx
#include <stdio.h>

#include <tchar.h>
#include <Psapi.h>
#include <iostream>
#include <conio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <vector>
#include <tlhelp32.h>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <exception>
#include "MD5New.h"
#include "MD5.h"
#include "operproxy.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>

#include <QListView>
#include <QTreeView>
#include <QDialogButtonBox>
#include<QFileIconProvider>
#pragma warning( disable : 4996)
#include <winsock2.h>
  #include <ws2tcpip.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <QListView>
#include <QTreeView>
#include <QDialogButtonBox>

#include "procfuncinfo.h"

extern GetR0ProcessInfo GetR0ProcessInfo_;

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#pragma comment (lib,"Psapi.lib")
#pragma comment(lib,"ws2_32.lib")

extern RepairNet RepairNet_;
extern char path[MAX_PATH];
extern StartVirtual StartVirtual_;
extern StopVirtual StopVirtual_;
extern SetProxyInfo SetProxyInfo_;
bool GetMacByGetAdaptersInfo(std::string& macOUT, std::string &name);
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);

int CFunction::delProcConf(int id)
{
//    qDebug() << "delProcConf id=" << id;

    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\procinfo.txt";
//    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";
    bool isfind = false;
    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
            isfind = true;
            qDebug() << "id=" << resid << " can find";
      }
      else
      {

        QString strline = oneline[0] +"|"+oneline[1]
                            +"|"+oneline[2] +"|"+oneline[3]
                            +"|"+oneline[4] +"|"+oneline[5]
                            +"|"+oneline[6] +"|"+"|"+oneline[7]
                            +"|"+oneline[8]+"|"+oneline[9]
                            +"|"+oneline[10];
        strres=strres+strline+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return -1;
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    if (isfind == true)
        return 0;
    else
        return -1;

    return 0;
}

int CFunction::resetlsp()
{
    QString strfillpath = path;
    strfillpath += "\\repairlsp.bat";

//    qDebug() << "strfillpath=" << strfillpath;
    // 使用ShellExecute函数运行脚本
    return WinExec((LPCSTR)strfillpath.toLocal8Bit().data(), SW_SHOWDEFAULT);
    // int res = std::system((LPCSTR)strfillpath.toLocal8Bit().data());this is also ok
}


CFunction::CFunction(QObject *parent):QObject(parent)
{
    QString strfillpath = path;
    QJsonObject json;
    file_dialog.setFileMode(QFileDialog::Directory);
    strfillpath += "\\setting.ini";
    settings = new QSettings(strfillpath,QSettings::IniFormat);
    connect(&file_dialog,&QFileDialog::currentChanged,this,[&](const QString & str)
      {
      QFileInfo info(str);
      if(info.isFile())
      {
             file_dialog.setFileMode(QFileDialog::ExistingFile);
//          file_dialog.setFileMode(QFileDialog::Directory);
             qDebug("file");
      }
      else if(info.isDir())
      {
          qDebug("dir");
          file_dialog.setFileMode(QFileDialog::Directory);
      }

  });

    strfillpath = path;
    strfillpath += "\\highlevelsetting.ini";
    highlevelsettings = new QSettings(strfillpath,QSettings::IniFormat);
    connect(&file_dialog,&QFileDialog::currentChanged,this,[&](const QString & str)
      {
      QFileInfo info(str);
      if(info.isFile())
      {
             file_dialog.setFileMode(QFileDialog::ExistingFile);
//          file_dialog.setFileMode(QFileDialog::Directory);
             qDebug("file");
      }
      else if(info.isDir())
      {
          qDebug("dir");
          file_dialog.setFileMode(QFileDialog::Directory);
      }

  });

//     settings->setIniCodec(QTextCodec::codecForName("UTF-8"));
}


int CFunction::addDragInfo(QString strName)
//int CFunction::addDragInfo(int type)
{

    QString fileName = QFileInfo(strName).fileName();
    qDebug() << "addDragInfo name=" << strName;
    qDebug() << "fileName=" << fileName;
//    addProc(fileName, strName);
    return 0;
}

int CFunction::addProcInfo(int type)
{
    int ret = -1;
    vector <string> vecstr;
    if (0 == type)
    {
        vecstr.push_back("LdBoxHeadless.exe");
        vecstr.push_back("LdVBoxHeadless.exe");
        vecstr.push_back("Ld9BoxHeadless.exe");
    }
    else if (1 == type)
    {
        vecstr.push_back("MEmuHeadless.exe");
    }
    else if (2 == type)
    {
        vecstr.push_back("NoxVMHandle.exe");
    }
    else if (3 == type)
    {
        vecstr.push_back("LsBoxHeadless.exe");
    }
    else if (4 == type)
    {
        vecstr.push_back("HD-Player.exe");
//        procleishenmsg("HD-Player.exe");
    }
    else if (5 == type)
    {
        vecstr.push_back("NemuHeadless.exe");
        vecstr.push_back("MuMuVMMHeadless.exe");
    }

    ret = procleishenmsg(vecstr);

    return 0;
}

int CFunction::addProccess(QString strname)
{
   QFileInfo fileInfo(strname);
   QFileIconProvider seekIcon;
   QIcon icon = seekIcon.icon(fileInfo);
    QPixmap pixmap=icon.pixmap(QSize(30,30));
    QString filename = fileInfo.baseName() + "_icon.png";
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\";
    strfillpath += filename;
    qDebug()<<"CFunction::addProccess filename111=" << fileInfo.baseName() + "_icon.png";
    if(!pixmap.save(strfillpath, "PNG"))
    {
       qDebug()<<"Save icon failed!!";
    }

    QStringList listinfo = strname.split("/");//QString字符串分割函数

    qDebug("cc=%d", listinfo.count());

    addProc(listinfo[listinfo.count()-1], strname, strfillpath, 0);
    return 0;
}

QString CFunction::addMultiProcInfo(QString strname)
{
//    procleishenmsg(strname);
    return "添加进程成功";
}

CFileDialog::CFileDialog(QWidget *parent)
    : QFileDialog(parent)
{
    this->setOption(QFileDialog::DontUseNativeDialog,true);

//        //支持多选
//        QListView *pListView = this->findChild<QListView*>("listView");
//        if (pListView)
//            pListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        QTreeView *pTreeView = this->findChild<QTreeView*>();
        if (pTreeView)
            pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        QDialogButtonBox *pButton = this->findChild<QDialogButtonBox *>("buttonBox");

        disconnect(pButton, SIGNAL(accepted()), this, SLOT(accept()));//使链接失效
        connect(pButton, SIGNAL(accepted()), this, SLOT(onChiose()));//改成自己的槽
}

CFileDialog::~CFileDialog()
{

}

void CFileDialog::onChiose()
{
    QDialog::accept();
}

#include<QListView>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QTranslator>
#include <QPushButton>
#include <QDialogButtonBox>
//#pragma execution_character_set("utf-8")
MultiFileDirDialog::MultiFileDirDialog(QWidget *parent)
    : QFileDialog(parent)
{

//    setOption(QFileDialog::DontUseNativeDialog, true);
    //支持多选
//    QListView *listView = findChild<QListView*>("listView");
//    if (listView)
//        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QTreeView *treeView = findChild<QTreeView*>();
    if (treeView)
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QDialogButtonBox *button = findChild<QDialogButtonBox *>("buttonBox");

    if (button)
    {
        /*QPushButton* selectbutton = button->button(QDialogButtonBox::Open);
        if (selectbutton)
        {
            button->removeButton(selectbutton);
            const QString strselect = "选择";
            selectbutton = button->addButton(QDialogButtonBox::Ok);
            selectbutton->setText("选择");
        }
        QPushButton* canclebutton = button->button(QDialogButtonBox::Cancel);
        if (canclebutton)
        {
            button->removeButton(canclebutton);
            const QString strcalcel = "取消";
            canclebutton=button->addButton(strcalcel, QDialogButtonBox::RejectRole);
            canclebutton->setText("取消");
        }*/
        button->clear();
        QPushButton* accpetbutton = new QPushButton(button);
        accpetbutton->setText("确定");
        button->addButton((QAbstractButton*)accpetbutton, QDialogButtonBox::AcceptRole);
        QPushButton* cancel = new QPushButton(button);
        cancel->setText("取消");
        button->addButton((QAbstractButton*)cancel, QDialogButtonBox::RejectRole);
        /*button->button(QDialogButtonBox::Ok)->setText(tr("确定"));
        button->button(QDialogButtonBox::Cancel)->setText(tr("取消"));*/
        disconnect(button, SIGNAL(accepted()), this, SLOT(accept()));//使链接失效
        connect(button, SIGNAL(accepted()), this, SLOT(go()));//改成自己的槽
    }
    //无效
    //setLabelText(QFileDialog::FileName, "选择文件:");//英文化文件名称
    //setLabelText(QFileDialog::Reject, "取消");//英文化取消按钮
    //setLabelText(QFileDialog::Accept, "选择");//英文化打开按钮


}

MultiFileDirDialog::~MultiFileDirDialog()
{

}
void  MultiFileDirDialog::go()
{
    m_listSelect = selectedFiles();
    QDialog::accept();
}

int CFunction::add()
{
//    return QDialog::accept();
    return 0;
}

int CFunction::captureProc(int type)
{
//    qDebug("iscapture=%d\n", type);

    ProcFuncThd * m_cpyThread;
    m_cpyThread = new ProcFuncThd(type, this);
    m_cpyThread->start();
    return 0;
}

int CFunction::delfile()
{
//    qDebug("del begin");
    QFile::remove("C:\\Windows\\JSBProcess.sys");
    QFile::remove("C:\\Windows\\JSBProxyA0.sys");
    QFile::remove("C:\\Windows\\JSBProxyW0.sys");
//    qDebug("del end");
    return 0;

}

int CFunction::repairnet()
{

   return  RepairNet_();

}

int CFunction::resethost()
{
    qDebug("resethost");
//    long res = SetFileRight((char *)"C:\\Windows\\System32\\drivers\\etc\\hosts");
//    qDebug("res=%d S_OK=%d S_FALSE=%d", res, S_OK, S_FALSE);
    QString strfillpath = path;
    strfillpath += "\\hosts";
    QFile::remove("C:\\Windows\\System32\\drivers\\etc\\hosts");
//////    CopyFile((LPCWSTR)"F:\\psc.jpg", (LPCWSTR)"C:\\Windows\\System32\\drivers\\etc\\psc2.jpg", false);
    std::ifstream  src(strfillpath.toLocal8Bit().data(), std::ios::binary);
    std::ofstream  dst("C:\\Windows\\System32\\drivers\\etc\\hosts", std::ios::binary);
    dst << src.rdbuf();
    return 0;
}

int CFunction::addPath()
{

    qDebug() << "addPath";
//    QFileDialog* dlg = new QFileDialog(NULL);
//    dlg->setFileMode(QFileDialog::Directory);
//    int nMode = dlg->exec();
//    QDialog dlg;
//    QString sPath = QFileDialog::getOpenFileName(
//                  &dlg,//指定父窗口
//                  "标准文件对话框",//打开文件对话框的标题
//                  ".",//打开目录，"." 表示当前目录
//                  "C++ files(*.cpp);;"
//                   "C files(*.c);;"
//                  "Header files(*.h)"//设置文件过滤器，有多个条件时中间以两个;;隔开
//                  );

//    CFileDialog fileDialog;
////    fileDialog.exec();
//    CFunction::add();
//    int ret = fileDialog.exec();
//    qDebug("ret=%d", ret);
//    MultiFileDirDialog dlg;
//    //m_pMultiFileDialog->setFileMode(QFileDialog::AnyFile);
//   dlg.setOption(QFileDialog::ShowDirsOnly, false);
//   dlg.setNameFilter("vedio(*.mp4 *.avi *.dav *.asf *.wmv *.mpg *.ts *.3g2 *.swf *.flv *.m2ts *.mkv *.3gp *.vob *.rmvb *.mov *.webm *.rm *.mts *.mpeg *.264 *.692 *.800 *.801 *.av *.avx *.djl *.dvr *.g64 *.h3crd *.h64 *.h264 *.ifv *.jmv *.kyd *.lvf *.mpk *.nsf *.nv4 *.ps *.rec *.sdv *.sv5 *.swf *.tm4 *.tp *.bhd *.dat *.h265 *.he4 *.m4c *.pep *.smf *.sv4 *.vdg *.601 *.452094848)");

//    dlg.setOption(QFileDialog::ShowDirsOnly, true);

//    if (dlg.exec() == QDialog::Accepted)
//      {
//          QStringList selectFiles = dlg.selectedFiles();
//          if (selectFiles.size() > 0)
//          {
//              QString strItem = "";
//              for (int i = 0; i < selectFiles.size(); i++)
//              {
//                  strItem = selectFiles[i];
//                  if (!strItem.isEmpty())
//                  {
////                      if (m_TaskFileMap.find(strItem) == m_TaskFileMap.end())
////                      {
////                          addFileToTable(strItem);
////                      }
//                  }

//              }
//          }
//      }

//    if(file_dialog.exec()){
//        QStringList dir_names=file_dialog.selectedFiles();
//    }


    FileDialog fileDialog(this);
    fileDialog.show();


//    FileDialog

//    QFileDialog dialog;
//    dialog.setFileMode(QFileDialog::DirectoryOnly);
//    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
//    dialog.setOption(QFileDialog::ShowDirsOnly, false);
//    dialog.exec();
//    QDialogButtonBox *pButton = this->findChild<QDialogButtonBox *>("buttonBox");

//    disconnect(pButton, SIGNAL(accepted()), this, SLOT(accept()));//使链接失效
//    connect(pButton, SIGNAL(accepted()), this, SLOT(onChiose()));//改成自己的槽
//    QDesktopServices::openUrl(QUrl("file:///C:/Documents and Settings/All Users", QUrl::TolerantMode));

//    QProcess::execute("explorer c:\\"); //只适用windows

//    if ( fileDialog.exec() == )
//    {
//         qDebug() << fileDialog.selectedFiles();
//    }
//    if ( fileDialog.exec() == QDialog::accept())
//    {
//         qDebug() << fileDialog.selectedFiles();
//    }

//    QFileDialog* dlg = new QFileDialog(NULL);
////    dlg->setFileMode(QFileDialog::Directory);
////    int nMode = dlg->exec();
////    QStringList strlist = dlg->selectedFiles();

////    QStringList strlist = dlg->selectedFiles();
//    QString dir = QFileDialog::getExistingDirectory(dlg, tr("Open Directory"),
//                                                "/home",
//                                                 QFileDialog::ShowDirsOnly
//                                                 | QFileDialog::DontResolveSymlinks);

//    if (0 == type)
//    {
//        procleishenmsg();
//        QString tmpdata = "";
//        tmpdata = tmpdata+"112|"+"332|"+"33|"+"11223|"+"112|"+"223|"+"221|"+"????|"+"112|"+"112";
//        return 0;
//    }

    return 0;
}

QString CFunction::addProc(QString procname, QString procpath, QString imagepath, int needmd5)
{
    qDebug() << "addProc="+procpath;
    QString strfillpath = path;
    strfillpath += "\\procinfo.txt";
    int idx = 1;

    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"File open failed";
    }
    else
    {
        QFile fileread(strfillpath);
        if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug()<<"File open failed";
        QTextStream in(&fileread);


        QString lines = "";
        while(!in.atEnd())
        {
            lines=in.readLine();
        }

//        qDebug() << "lines111=" << lines;
        if (lines != "")
        {
            qDebug() << "lines112=" << lines;
            QStringList strlist=lines.split("|");
            if (strlist.size() > 0)
            {
                idx = atoi(strlist[0].toUtf8().data());

                idx++;
                 qDebug() << "idx11221=" << idx;
            }

        }

    }

    fileread.close();

    qDebug() << "lines113=";

    QFile file(strfillpath);

    bool isok=file.open(QIODevice::ReadWrite | QIODevice::Append);//
    if(!isok)
    {
        qDebug() << "write error";
    }

        qDebug() << "lines114=";
    string strmd5 = "";
    if (1 == needmd5)
   strmd5 = getFileMD5(procpath.toStdString());
    QString strtmp;
       strtmp= strtmp+procname + "|" + strmd5.c_str() + "|" + procpath + "|" + "0" + "|"
               + "0" + "|" + "true" + "|"
               + "false" + "|" + "false" + "|"
               + "128"+ "|" + "0"
               + "|" + ""
               + "|" + ""
               +"|" + ""+
               +"|" + +"|" + imagepath+"\n";
//       qDebug() << "strtmp=" << strtmp;

    QString stridx = QString::number(idx);
    QString strwrite = stridx + "|" + strtmp;


    file.write(strwrite.toStdString().data());
    file.close();
    return "";
}

int getconnnum(char *port)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    _unlink("c:/aaa.log");

    HANDLE h = CreateFile((L"c:/aaa.log"),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret = FALSE;
    DWORD flags = CREATE_NO_WINDOW;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = h;
    si.hStdOutput = h;

    TCHAR cmd[] = TEXT("netstat -any");
    ret = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);

    if (ret)
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
//        printf("succeed....\n");

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        //关闭文件
        CloseHandle(h);
        int cnt = 0;
        FILE *fp = fopen("c:/aaa.log", "r");
        if (fp)
        {
            char RdStrng[1000];

            while (fgets(RdStrng, 1000, fp) != NULL)
            {
//                printf("%s", RdStrng);
                if (strstr(RdStrng, "ESTABLISHED") != NULL && strstr(RdStrng, port) != NULL)
                {
                    cnt++;
                }
            }

            fclose(fp);
//            qDebug("cnt=%d", cnt);
        }
        return cnt;
    }
    //关闭文件
    CloseHandle(h);
//    printf("failure....\n");
    return 0;
}

int getpidbyport(char *port, char *pid)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    _unlink("c:/aaa.log");

    HANDLE h = CreateFile((L"c:/aaa.log"),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret = FALSE;
    DWORD flags = CREATE_NO_WINDOW;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = h;
    si.hStdOutput = h;

    TCHAR cmd[] = TEXT("netstat -ano");
    ret = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);

    if (ret)
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
//        printf("succeed....\n");

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        //关闭文件
        CloseHandle(h);
        int cnt = 0;
        FILE *fp = fopen("c:/aaa.log", "r");
        if (fp)
        {
            char RdStrng[1000];

            while (fgets(RdStrng, 1000, fp) != NULL)
            {
//                printf("%s", RdStrng);
//                if (strstr(RdStrng, "ESTABLISHED") != NULL && strstr(RdStrng, port) != NULL)
                                if (strstr(RdStrng, "ESTABLISHED") != NULL
                                        && strstr(RdStrng, pid) != NULL)
                {
                    qDebug() << "we get line=" << RdStrng;
                    cnt++;
                }
            }

            fclose(fp);
//            qDebug("cnt=%d", cnt);
        }
        return cnt;
    }
    //关闭文件
    CloseHandle(h);
//    printf("failure....\n");
    return 0;
}

BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
    TCHAR            szDriveStr[500];
    TCHAR            szDrive[3];
    TCHAR            szDevName[100];
    INT                cchDevName;
    INT                i;

    if(!pszDosPath || !pszNtPath )

        return FALSE;

    if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
    {
        for(i = 0; szDriveStr[i]; i += 4)
        {
            if(!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
                continue;
            szDrive[0] = szDriveStr[i];
            szDrive[1] = szDriveStr[i + 1];
            szDrive[2] = '\0';
            if(!QueryDosDevice(szDrive, szDevName, 100))
                return FALSE;
            cchDevName = lstrlen(szDevName);
            if(_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)
            {
                lstrcpy(pszNtPath, szDrive);
                lstrcat(pszNtPath, pszDosPath + cchDevName);

                qDebug("DosPathToNtPath=%s", pszNtPath);
                return TRUE;
            }
        }
    }

    lstrcpy(pszNtPath, pszDosPath);

    return FALSE;

}


BOOL GetProcessFullPath(DWORD dwPID,string &fullPath, string &imagepath)
{
    TCHAR		szImagePath[MAX_PATH];
    TCHAR		pszFullPath[MAX_PATH];
    HANDLE		hProcess;

    if(!pszFullPath)
    {
        return FALSE;
    }
    pszFullPath[0] = '\0';

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
    if(!hProcess)
    {
        qDebug() << "data12=";
        return FALSE;
    }

    if(!GetProcessImageFileName(
        hProcess,
        szImagePath,
        MAX_PATH
    )){
        CloseHandle(hProcess);
        qDebug() << "data13=";
        return FALSE;
    }

    if(!DosPathToNtPath(szImagePath, pszFullPath))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    CloseHandle(hProcess);

     int iLen = 2 * wcslen(pszFullPath);
     char* chRtn = new char[iLen + 1];
     wcstombs(chRtn, pszFullPath, iLen + 1);

    fullPath = chRtn;

    QFileInfo fileInfo(chRtn);
    QFileIconProvider seekIcon;
    QIcon icon = seekIcon.icon(fileInfo);
     QPixmap pixmap=icon.pixmap(QSize(30,30));
     QString filename = fileInfo.baseName() + "_icon.png";
     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\";
     strfillpath += filename;
     qDebug()<<"filename111=" << fileInfo.baseName() + "_icon.png";
     if(!pixmap.save(strfillpath, "PNG"))
     {
        qDebug()<<"Save icon failed!!";
     }

     imagepath = strfillpath.toStdString();

    return TRUE;
}

BOOL CFunction::IsExistProcess(vector <string> &vecstr, string &strres, string &strname, string &imagepath)
{
    PROCESSENTRY32 processEntry32;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (((int)toolHelp32Snapshot) != -1)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
            do
            {
                int iLen = 2 * wcslen(processEntry32.szExeFile);
                char* chRtn = new char[iLen + 1];
                wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
                for (auto data:vecstr)
                {
                    if (strcmp(data.c_str(), chRtn) == 0)
                    {
                        GetProcessFullPath(processEntry32.th32ProcessID, strres, imagepath);

                        delete [] chRtn;
                        strname = data;
                        return TRUE;
                    }
                }
                delete [] chRtn;


            } while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }

        CloseHandle(toolHelp32Snapshot);
    }
    //
    return FALSE;
}

QVector<CProcInfo> CFunction::getpidinfo(const char *szProcessName)
{
    QVector<CProcInfo> vecdata;
    PROCESSENTRY32 processEntry32;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (((int)toolHelp32Snapshot) != -1)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
            do
            {
                int iLen = 2 * wcslen(processEntry32.szExeFile);
                char* chRtn = new char[iLen + 1];
                wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
//                qDebug("size111");
                if (strcmp(szProcessName, chRtn) == 0)
                {
//                    qDebug("size112");
//                    pid = processEntry32.th32ProcessID;
                    CProcInfo procinfo;
                    procinfo.pid = processEntry32.th32ProcessID;
                    procinfo.strname = chRtn;
                    vecdata.push_back(procinfo);
//                    delete [] chRtn;
//                    return TRUE;
                }

                delete [] chRtn;
            } while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }

        CloseHandle(toolHelp32Snapshot);
    }

    return vecdata;
    //
//    return FALSE;
}

QString CFunction::readProcConf()
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\procinfo.txt";
//    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);

    int idx = 0;
    int id = 0;
    QString dev;

    while(!in.atEnd())
    {
        QJsonObject pageObject;
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
        pageObject.insert("id", oneline[0].trimmed().toLocal8Bit().data());
        pageObject.insert("proc", oneline[1].trimmed().toLocal8Bit().data());
        pageObject.insert("MD5", oneline[2].trimmed().toLocal8Bit().data());
        pageObject.insert("path", oneline[3].trimmed().toLocal8Bit().data());
        pageObject.insert("proxymode", oneline[4].trimmed().toLocal8Bit().data());
        pageObject.insert("stratery", oneline[5].trimmed().toLocal8Bit().data());
        pageObject.insert("subproc", oneline[6].trimmed().toLocal8Bit().data());
        pageObject.insert("virtual", oneline[7].trimmed().toLocal8Bit().data());
        pageObject.insert("limit", oneline[8].trimmed().toLocal8Bit().data());
        pageObject.insert("filterstatus",
                          oneline[10].trimmed().toLocal8Bit().data());

        dev = "proc" + QString::number(idx);

        json.insert(dev, QJsonValue(pageObject));
        idx++;
    }

    fileread.close();
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    return strJson;
}


std::string CFunction::getFileMD5(const std::string& filename)
{
    std::ifstream fin(filename.c_str(), std::ifstream::binary);
    if (fin)
    {
        MD5_CTX context;
        MD5Init(&context);
//        qDebug() << "getFileMD5:1";
        fin.seekg(0, fin.end);
        const auto fileLength = fin.tellg();
        fin.seekg(0, fin.beg);

//        const int bufferLen = 8192;
//        std::unique_ptr<unsigned char[]> buffer{ new unsigned char[bufferLen] {} };
        unsigned long long totalReadCount = 0;
//        decltype(fin.gcount()) readCount = 0;
        unsigned char buffer[64 * 1024];

        // 读取文件内容，调用MD5Update()更新MD5值
//        while (fin.read(reinterpret_cast<char*>(buffer.get()), bufferLen))
//        {
//            readCount = fin.gcount();
//            totalReadCount += readCount;
//            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
//        }

        while (!fin.eof())
        {
          fin.read((char *)buffer, sizeof(buffer));
          totalReadCount += fin.gcount();
          MD5Update(&context, buffer, fin.gcount());
        }


        // 处理最后一次读到的数据
//        int readCount = fin.gcount();
//        qDebug() << "getFileMD5:2 readCount=" <<readCount << "total=" <<readCount+totalReadCount;
//        if (readCount > 0)
//        {
//            totalReadCount += readCount;
//            MD5Update(&context, buffer, fin.gcount());
////            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
//        }
        fin.close();
//        qDebug() << "getFileMD5:3" << totalReadCount << " fileLength=" << fileLength;
        // 数据完整性检查
//        if (totalReadCount != fileLength)
//        {
//            qDebug() << "getFileMD5:3.1";
//            std::ostringstream oss;
//            oss << "FATAL ERROR: read " << filename << " failed!" << std::ends;
//            throw std::runtime_error(oss.str());
//        }

//        qDebug() << "getFileMD5:3.2";

        unsigned char digest[16];
        MD5Final(digest, &context);
//        qDebug() << "getFileMD5:3.3";
        // 获取MD5
        std::ostringstream oss;
        for (int i = 0; i < 16; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(digest[i]);
        }
        oss << std::ends;
//        qDebug() << "getFileMD5:4";
        return std::move(oss.str());
    }
    else
    {
        std::ostringstream oss;
        oss << "FATAL ERROR: " << filename << " can't be opened" << std::ends;
        throw std::runtime_error(oss.str());
    }
}

//int CFunction::procleishenmsg(QString strName)
int CFunction::procleishenmsg(vector <string> &vecstr)
//int CFunction::procleishenmsg()
{
    string strpath;
    string strname;
    string imagepath;
    if (TRUE == IsExistProcess(vecstr, strpath, strname, imagepath))
    {
        addProc(strname.c_str(), strpath.c_str(), imagepath.c_str(), 1);
        return 0;
    }
    else
    {
        return -1;
    }


}

QString CFunction::getOneRowData(QString id)
{
    QString strfillpath = path;
    QJsonObject json;
    qDebug()<<"getOneRowData 111";
    strfillpath += "\\procinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);

    int idx = 0;
    QString dev;
    getConfInfo();
    while(!in.atEnd())
    {
        qDebug()<<"getOneRowData 22";
        QJsonObject pageObject;
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
        if (id == oneline[0])
        {
           pageObject.insert("id", oneline[0].trimmed().toLocal8Bit().data());
           pageObject.insert("proc", oneline[1].trimmed().toLocal8Bit().data());
           pageObject.insert("MD5", oneline[2].trimmed().toLocal8Bit().data());
           pageObject.insert("path", oneline[3].trimmed().toLocal8Bit().data());
           pageObject.insert("proxymode", oneline[4].trimmed().toLocal8Bit().data());
           pageObject.insert("stratery", oneline[5].trimmed().toLocal8Bit().data());
           pageObject.insert("subproc", oneline[6].trimmed().toLocal8Bit().data());
           pageObject.insert("virtual", oneline[7].trimmed().toLocal8Bit().data());
           pageObject.insert("limit", oneline[8].trimmed().toLocal8Bit().data());
           pageObject.insert("limitval", oneline[9].trimmed().toLocal8Bit().data());
           pageObject.insert("filter", oneline[10].trimmed().toLocal8Bit().data());
           QString str = oneline[11];
           qDebug()<<"getOneRowData 33";
           if(str.contains("-",Qt::CaseSensitive))
           {
                str = str.replace("-", "\n");
                 qDebug()<<"getOneRowData 44";
           }

             qDebug()<<"getOneRowData 55";
           pageObject.insert("destaddr", str.trimmed().toLocal8Bit().data());
           qDebug()<<"getOneRowData 66";
           pageObject.insert("textclassname", oneline[12].trimmed().toLocal8Bit().data());
            qDebug()<<"getOneRowData 77";
           pageObject.insert("textbeforeid", oneline[13].trimmed().toLocal8Bit().data());
           qDebug()<<"getOneRowData 88";
           pageObject.insert("textafterid", oneline[14].trimmed().toLocal8Bit().data());
            qDebug()<<"getOneRowData 99";
           dev = "proc" + QString::number(idx);
           json.insert(dev, QJsonValue(pageObject));
           qDebug()<<"id1111=" << id << " oneline=" << oneline[0];
           idx++;
           break;
        }
    }
    qDebug()<<"getOneRowData 33";
    fileread.close();
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
//    qDebug() << "data="+strJson;
    return strJson;
}

int CFunction::opendir(QString id)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\procinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);

    int idx = 0;
    QString dev;
    getConfInfo();
    while(!in.atEnd())
    {
        QJsonObject pageObject;
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
        if (id == oneline[0])
        {

           string str = oneline[3].trimmed().toLocal8Bit().data();
            int idx = str.find_last_of("\\");
            string substr1 = str.substr(0, idx);
            qDebug("str11=%s", substr1.c_str());
            QDesktopServices::openUrl(QUrl::fromLocalFile(substr1.c_str()));
            return 0;

        }
    }


    return 0;
}

int CFunction::isbannetwork()
{
    QString isbannetworkinfo = settings->value("functionsetting/isbannetwork").toString();
    return isbannetworkinfo.toInt();
}

 int CFunction::getdrivertype()
{
     QString driverkernelmode = highlevelsettings->value("mode/driverkernelmode").toString();
    if (driverkernelmode == "1")
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


 QString CFunction::getHighLevelConfInfo()
 {
     QString setprocpri = highlevelsettings->value("global/setprocpri").toString();
     QString detquerythd = highlevelsettings->value("generate/detquerythd").toString();
     QString timeout = highlevelsettings->value("generate/timeout").toString();

     QString latencydettimeout = highlevelsettings->value("generate/latencydettimeout").toString();

     QString bindwindowtimeout = highlevelsettings->value("generate/bindwindowtimeout").toString();
     QString chkproxyavail = highlevelsettings->value("generate/chkproxyavail").toString();

     QString deccycle = highlevelsettings->value("generate/deccycle").toString();


     QString driverkernelmode = highlevelsettings->value("mode/driverkernelmode").toString();

     QString generatemode = highlevelsettings->value("mode/generatemode").toString();
     QString driverprocessmode = highlevelsettings->value("mode/driverprocessmode").toString();

     QString commonprocessmode = highlevelsettings->value("mode/commonprocessmode").toString();

     QJsonObject pageObject;

     pageObject.insert("setprocpri", setprocpri.toLocal8Bit().data());
     pageObject.insert("detquerythd", detquerythd.toLocal8Bit().data());
     pageObject.insert("timeout", timeout.toLocal8Bit().data());
     pageObject.insert("latencydettimeout", latencydettimeout.toLocal8Bit().data());
     pageObject.insert("bindwindowtimeout", bindwindowtimeout.toLocal8Bit().data());

     pageObject.insert("chkproxyavail", chkproxyavail.toLocal8Bit().data());
     pageObject.insert("deccycle", deccycle.toLocal8Bit().data());

     pageObject.insert("driverkernelmode", driverkernelmode.toLocal8Bit().data());
     pageObject.insert("generatemode", generatemode.toLocal8Bit().data());

     pageObject.insert("driverprocessmode", driverprocessmode.toLocal8Bit().data());
     pageObject.insert("commonprocessmode", commonprocessmode.toLocal8Bit().data());

     int idx = 0;
     QString dev;
     dev = "proc" + QString::number(idx);
      QJsonObject json;
     json.insert(dev, QJsonValue(pageObject));
     QJsonDocument document;
     document.setObject(json);
     QByteArray byteArray = document.toJson(QJsonDocument::Compact);
     QString strJson(byteArray);
     return strJson;
 }


QString CFunction::getConfInfo()
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\setting.ini";
    QFile file(strfillpath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return "";
    }

    QByteArray buff= file.readAll();//读取所有内容

    QString path = settings->value("Identificationmethod/path").toString();
    QString processMD5 = settings->value("Identificationmethod/processMD5").toString();

    QString isbannetwork = settings->value("functionsetting/isbannetwork").toString();

    QString issubsameaddr = settings->value("functionsetting/issubsameaddr").toString();
    QString iscapture = settings->value("functionsetting/iscapture").toString();

    QString packetvitual = settings->value("functionsetting/packetvitual").toString();
    QString issubsamehardaddr = settings->value("functionsetting/issubsamehardaddr").toString();

    QString issuspendprocess = settings->value("functionsetting/issuspendprocess").toString();
    QString processnoflowtime = settings->value("functionsetting/processnoflowtime").toString();

    QString isstart = settings->value("functionsetting/isstart").toString();
    QString ismin = settings->value("functionsetting/ismin").toString();

    QString istrap = settings->value("functionsetting/istrap").toString();
    QString title = settings->value("functionsetting/title").toString();
    QString procnameinfo = settings->value("Identificationmethod/processname").toString();

    QString dnsip = settings->value("mode/dnsip").toString();
    QString port = settings->value("mode/port").toString();
    QString parsetype = settings->value("mode/parsetype").toString();

    QString openproxy = settings->value("functionsetting/openproxy").toString();
    QString firewallprohibits = settings->value("functionsetting/firewallprohibits").toString();
    QString vmmac = settings->value("functionsetting/vmmac").toString();
    QString vmdiskserial = settings->value("functionsetting/vmdiskserial").toString();
    QString vmdvolumelabel = settings->value("functionsetting/vmdvolumelabel").toString();
    QString vmdRegistryFeatures = settings->value("functionsetting/vmdRegistryFeatures").toString();
    QString vmProtectionIsolation = settings->value("functionsetting/vmProtectionIsolation").toString();




    QString proxychildproc = settings->value("functionsetting/proxychildproc").toString();
    QString virtualhardinfo = settings->value("functionsetting/virtualhardinfo").toString();

//    qDebug() << "222 str=" << title;
//    qDebug() << "procnameinfo=" << procnameinfo << " isbannetwork=" << isbannetwork
//                << " issubsameaddr=" << issubsameaddr<< " iscapture=" << iscapture
//                << " packetvitual=" << packetvitual<< " issubsamehardaddr=" << issubsamehardaddr
//                << " issuspendprocess=" << issuspendprocess<< " processnoflowtime=" << processnoflowtime
//                << " istrap=" << istrap<< " title1111=" << title.toLocal8Bit().data() << " path=" << path
//                << " processMD5=" << processMD5;

    QJsonObject pageObject;
    pageObject.insert("processname", procnameinfo.toLocal8Bit().data());
    pageObject.insert("path", path.toLocal8Bit().data());
    pageObject.insert("processMD5", processMD5.toLocal8Bit().data());

    pageObject.insert("isbannetwork", isbannetwork.toLocal8Bit().data());
    pageObject.insert("issubsameaddr", issubsameaddr.toLocal8Bit().data());
    pageObject.insert("iscapture", iscapture.toLocal8Bit().data());
    pageObject.insert("packetvitual", packetvitual.toLocal8Bit().data());
    pageObject.insert("issubsamehardaddr", issubsamehardaddr.toLocal8Bit().data());
    pageObject.insert("issuspendprocess", issuspendprocess.toLocal8Bit().data());
    pageObject.insert("processnoflowtime", processnoflowtime.toLocal8Bit().data());

    pageObject.insert("isstart", isstart.toLocal8Bit().data());
    pageObject.insert("ismin", ismin.toLocal8Bit().data());
    pageObject.insert("istrap", istrap.toLocal8Bit().data());
    pageObject.insert("title", title.toStdString().c_str());

    pageObject.insert("firewallprohibits", firewallprohibits.toLocal8Bit().data());
    pageObject.insert("openproxy", openproxy.toLocal8Bit().data());

    pageObject.insert("dnsip", dnsip.toLocal8Bit().data());
    pageObject.insert("port", port.toLocal8Bit().data());
    pageObject.insert("parsetype", parsetype.toStdString().c_str());
    pageObject.insert("dnsip", dnsip.toStdString().c_str());
    pageObject.insert("port", port.toStdString().c_str());
    pageObject.insert("vmmac", vmmac.toStdString().c_str());
    pageObject.insert("vmdiskserial", vmdiskserial.toStdString().c_str());
    pageObject.insert("vmdvolumelabel", vmdvolumelabel.toStdString().c_str());
    pageObject.insert("vmdRegistryFeatures", vmdRegistryFeatures.toStdString().c_str());
    pageObject.insert("vmProtectionIsolation", vmProtectionIsolation.toStdString().c_str());


    pageObject.insert("proxychildproc", proxychildproc.toStdString().c_str());
    pageObject.insert("virtualhardinfo", virtualhardinfo.toStdString().c_str());


    int idx = 0;
    QString dev;
    dev = "proc" + QString::number(idx);
    json.insert(dev, QJsonValue(pageObject));
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    return strJson;
}

bool GetMacByGetAdaptersInfo(std::string& macOUT, std::string &name)
{
    bool ret = false;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));

    if (pAdapterInfo == NULL)
        return false;

    // Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (pAdapterInfo == NULL)
            return false;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
    {
        for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
        {
            // 确保是以太网
            if (pAdapter->Type != MIB_IF_TYPE_ETHERNET)
                continue;

            // 确保MAC地址的长度为 00-00-00-00-00-00
            if (pAdapter->AddressLength != 6)
                continue;

            char acMAC[32];
            sprintf_s(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
                int(pAdapter->Address[0]),
                int(pAdapter->Address[1]),
                int(pAdapter->Address[2]),
                int(pAdapter->Address[3]),
                int(pAdapter->Address[4]),
                int(pAdapter->Address[5]));
            macOUT = acMAC;
            name =  pAdapter->AdapterName;
//            qDebug("AdapterName=%s Description=%s", pAdapter->AdapterName, pAdapter->Description);
            ret = true;

            break;
        }
    }

    free(pAdapterInfo);
    return ret;
}

QVector<CProcInfo> CFunction::GetProceeIDfromParentID(unsigned long dwParentProcessId)
{
    QVector <CProcInfo> parent_id_vec;
    unsigned long dwProcessID = 0;
    PROCESSENTRY32 processEntry32;
    CProcInfo procInfo;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (((int)toolHelp32Snapshot) != -1)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
           do
           {
                if (processEntry32.th32ParentProcessID == dwParentProcessId)//判断如果父id与其pid相等
                {
                    procInfo.pid = processEntry32.th32ProcessID;
                    int iLen = 2 * wcslen(processEntry32.szExeFile);
                    char* chRtn = new char[iLen + 1];
                    memset(chRtn, 0, iLen+1);
                    wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
                    procInfo.strname = chRtn;
                    dwProcessID = processEntry32.th32ProcessID;
                    parent_id_vec.push_back(procInfo);
                    delete [] chRtn;
                }
           } while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }

        CloseHandle(toolHelp32Snapshot);
    }

    return parent_id_vec;
}

/*******************************
 * 时间：2021年12月1日
 * 功能：设置/取消 进程开机自动启动函数
 * 参数：
    1、appPath：需要设置/取消的自启动软件的“绝对路径”
    2、flag：   设置/取消自启动标志位，1为设置，0为取消,默认为设置
*******************************/
//注册表路径需要使用双反斜杠
//#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
void setProcessAutoRun(const QString &appPath,bool flag)
{
    QString application_name = "VMS";
       //QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
       QSettings *settings = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
       QString application_path = QDir::toNativeSeparators(QApplication::applicationFilePath());
       if (flag)
       {
           settings->setValue(application_name, application_path);
       }
       else
       {
           settings->remove(application_name);
       }
       delete settings;
}


char *CFunction::MD5Data(char * StrSrc)
{
    static char Rst[500] = { 0 };
    MD5 Md5;
    Md5.update(StrSrc);
    strcpy_s(Rst, Md5.toString().c_str());
    Md5.reset();

    return Rst;
}

char *iptos(u_long in)
{
    int IPTOSBUFFERS = 24;
    static char output[24][24];
    static short which;
    u_char *p;

    p = (u_char *)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
    sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output[which];
}

QString CFunction::reCapture(int isautodetect, QString strprocname)
{
    QString strisopen = isopenproxy();
//    qDebug() << "strisopen=" << strisopen;
    if ("0" == strisopen)
        return "";

    QString proxychildproc = settings->value("functionsetting/proxychildproc").toString();
    QString virtualhardinfo = settings->value("functionsetting/virtualhardinfo").toString();
    QString issubsameaddr = settings->value("functionsetting/issubsameaddr").toString();
    QString issubsamehardaddr = settings->value("functionsetting/issubsamehardaddr").toString();
//    qDebug() << "issubsamehardaddr=" << issubsamehardaddr;
//    return "";

    string maca;
    std::string name;
    QString strfillpath = path;
    QJsonObject json;

    strfillpath += "\\procinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
       qDebug()<<"File open failed";
    QVector <QString> vecStr = readProxyConf();
    QTextStream in(&fileread);

    QString dev;
//    getConfInfo();
    int idx = 1;
    int tmpcnt = 0;
    QString strLine = "";
    bool isexist = false;
    QMap <QString, QString> mapProc;

    QVector <captureinfo> veccap;
    QVector <QString> needcapdata;
    QMap <QString, QString> mapProcExist;
    while(!in.atEnd())
    {
        QString lines=in.readLine();
        QStringList oneline=lines.split("|");
        if (mapProcExist.find(oneline[1].toLocal8Bit().data()) != mapProcExist.end())
        {
            continue;
        }

        mapProcExist[oneline[1]] = oneline[1];

        if (isautodetect == 1)
        {

             if (oneline[1] == strprocname)
             {
                needcapdata.push_back(lines);
             }
        }
        else
        {
            needcapdata.push_back(lines);
        }
    }

    SchedulingStrategy schedulingStrategy(this);
    for (int i=0; i<needcapdata.size(); i++)
    {
        QJsonObject pageObject;
        QStringList oneline=needcapdata[i].split("|");
        QVector<CProcInfo> vecalldata = getpidinfo(oneline[1].toLocal8Bit().data());
         for (int i=0; i<vecalldata.size(); i++)
         {
             QString res = "";
             int ret = schedulingStrategy.GetProxyInfo(needcapdata, vecStr, res);
             if (ret)
             {
                return "";
             }

//             qDebug() << "res=" << res;

            strLine = res;
             QStringList proxydata=strLine.split("|");
 //            qDebug("line=%s", lines.toStdString().c_str());
 //            qDebug("subproc=%s", oneline[6].toStdString().c_str());
 //            qDebug("vmhardware=%s", oneline[7].toStdString().c_str());

             QString s = QString::number(vecalldata[i].pid, 10);
             qDebug() << "pid=%s" << s;
             pageObject.insert("id", oneline[0].toLocal8Bit().data());
             pageObject.insert("pid", s.toLocal8Bit().data());
             pageObject.insert("proc", oneline[1].toLocal8Bit().data());
             pageObject.insert("connection", "0");
             pageObject.insert("sendnum", "0");
             pageObject.insert("recvnum", "0");
             pageObject.insert("isparent", "1");

//             qDebug() << "proxy port=" << proxydata[2];
             captureinfo capinfo;
             capinfo.port = proxydata[2].toInt();
             capinfo.id = oneline[0].toInt();
             capinfo.pid = vecalldata[i].pid;
             veccap.push_back(capinfo);
 //            veccap.append(capinfo);

             if (mapProc.find(oneline[1].toLocal8Bit().data()) != mapProc.end())
             {
                 qDebug("size44=%d", vecalldata.size());
                 isexist = true;
                 pageObject.insert("proxyip", "未匹配到");
                 pageObject.insert("mac", "N/A");
                 pageObject.insert("pos", "N/A");
             }
             else
             {
 //                qDebug("size55=%d", vecalldata.size());
                 pageObject.insert("proxyip", proxydata[1].toLocal8Bit().data());
                 if (virtualhardinfo == "1" || "true" == oneline[7])
                 {
//                     qDebug("true vm begin");
                     StartVirtual_((char *)"www.8u18.com");
                 }

                 GetMacByGetAdaptersInfo(maca, name);
                 if (virtualhardinfo == "1" || "true" == oneline[7])
                 {
//                     qDebug("true vm end");
                      StopVirtual_();
                 }

                 pageObject.insert("mac", maca.c_str());
                  pageObject.insert("pos", proxydata[7].toLocal8Bit().data());
                 isexist = false;
                  mapProc[oneline[1].toLocal8Bit().data()] = oneline[1].toLocal8Bit().data();
             }

 //            qDebug("size6=%d", vecalldata.size());

             dev = "proc" + QString::number(idx);
             json.insert(dev, QJsonValue(pageObject));
             idx++;
             if (proxychildproc == "1" || "true" == oneline[6])
             {
                 QVector<CProcInfo> vecdata = GetProceeIDfromParentID(vecalldata[i].pid);
                 for (auto data:vecdata)
                 {
                     QString s = QString::number(data.pid, 10);
                     QJsonObject pageObject;

                     if (issubsameaddr == "1")
                     {
                         int ret = schedulingStrategy.GetProxyInfo(needcapdata, vecStr, res);
                         if (ret)
                         {
                            return "";
                         }

//                         tmpcnt++;
//                         strLine = vecStr[tmpcnt];

                          strLine = res;
                         proxydata=strLine.split("|");
                     }


                     pageObject.insert("id", oneline[0].toLocal8Bit().data());
                     pageObject.insert("pid", s.toLocal8Bit().data());
                     pageObject.insert("proc", data.strname);

     //                pageObject.insert("connection", strconn.toLocal8Bit().data());
                     pageObject.insert("sendnum", "0");
                     pageObject.insert("recvnum", "0");
                     pageObject.insert("isparent", "0");
                     if (isexist == true)
                     {
                         QString strdesc = QString("因父 %1,禁网").arg(vecalldata[i].pid);
                         pageObject.insert("proxyip", strdesc);
                         pageObject.insert("mac", "N/A");
                         pageObject.insert("pos", "N/A");
                     }
                     else
                     {
                         pageObject.insert("proxyip", proxydata[1].toLocal8Bit().data());
                         if (issubsamehardaddr == "1" || virtualhardinfo == "1")
                            pageObject.insert("mac", maca.c_str());
                         else
                         {
                             if ("false" == oneline[7])
                             {
                                 qDebug("true vm begin");
                                 StartVirtual_((char *)"www.8u18.com");
                             }

                             GetMacByGetAdaptersInfo(maca, name);
                             if ("false" == oneline[7])
                             {
                                 qDebug("true vm end");
                                  StopVirtual_();
                             }
                         }
                         pageObject.insert("pos", proxydata[7].toLocal8Bit().data());
                     }
                     dev = "proc" + QString::number(idx);
                     json.insert(dev, QJsonValue(pageObject));
             //        qDebug()<<"id1111=" << id << " oneline=" << oneline[0];
                     idx++;
                 }
             }

             char KeyData[500] = { 0 };
             int DNSSwitch = 0;
         ////    DNSSwitch = 2;
//             QString DNSIP = "1.1.1.1";
//             int ProxyTypes = 0;
//             int DNSPort = 11112;
             QString ProcessName = oneline[1];

             QString dnsip = settings->value("mode/dnsip").toString();
             QString port = settings->value("mode/port").toString();
             QString parsetype = settings->value("mode/parsetype").toString();

             QString DNSIP = dnsip;
             int ProxyTypes = parsetype.toInt();
             int DNSPort = port.toInt();

             QString proxychildproc = settings->value("functionsetting/proxychildproc").toString();
             QString virtualhardinfo = settings->value("functionsetting/virtualhardinfo").toString();
             QString issubsameaddr = settings->value("functionsetting/issubsameaddr").toString();

             int isban = isbannetwork();
 //            qDebug() << "isban=" << isban;
             if (1 == isban)
              {
                 if (isexist == true)
                 {
                     QString strip = "191.168.43.62";
                     sprintf_s(KeyData, 500, "%s%s%s%d", ProcessName.toStdString().c_str(), strip.toLocal8Bit().data(), "K120R20K21e4yF0", GetCurrentThreadId() * 5 + 12242022);
                     if (SetProxyInfo_(ProxyTypes, DNSSwitch, (char *)DNSIP.toStdString().c_str(), DNSPort,
                                       (char *)ProcessName.toStdString().c_str(), vecalldata[i].pid,
                                       (char *)strip.toLocal8Bit().data(),
                                       11234, "user",
                                       "passwd", MD5Data(KeyData)))
                     {
                        qDebug("SetProxyInfo_ set wrong ip and port");
 //                        qDebug("SetProxyInfo_ succeed name=%s  pid=%d ip=%s port=%d username=%s passwd=%s ", ProcessName.toStdString().c_str(),
 //                               vecalldata[i].pid, (char *)strip.toLocal8Bit().data(), 11234,
 //                               "user", "passwd");

                     }
                     else
                     {
                         qDebug("SetProxyInfo_ failure");
                     }
                    continue;
                 }
                 else
                 {
 //                    qDebug() << "set right ip and port";
                     sprintf_s(KeyData, 500, "%s%s%s%d", ProcessName.toStdString().c_str(), proxydata[1].toLocal8Bit().data(), "K120R20K21e4yF0", GetCurrentThreadId() * 5 + 12242022);
                     if (SetProxyInfo_(ProxyTypes, DNSSwitch, (char *)DNSIP.toStdString().c_str(), DNSPort,
                                       (char *)ProcessName.toStdString().c_str(), vecalldata[i].pid,
                                       (char *)proxydata[1].toLocal8Bit().data(),
                                       atoi(proxydata[2].toLocal8Bit().data()), proxydata[3].toLocal8Bit().data(),
                                       proxydata[4].toLocal8Bit().data(), MD5Data(KeyData)))
                     {

                        qDebug("SetProxyInfo_ succeed");


 //                        qDebug("SetProxyInfo_ succeed name=%s  pid=%d ip=%s port=%s username=%s passwd=%s ", ProcessName.toStdString().c_str(),
 //                               vecalldata[i].pid, (char *)proxydata[1].toLocal8Bit().data(), proxydata[2].toLocal8Bit().data(),
 //                                proxydata[3].toLocal8Bit().data(), proxydata[4].toLocal8Bit().data());

                     }
                     else
                     {

                         qDebug("SetProxyInfo_ failure");
                     }

    //                 tmpcnt++;lgh
                 }
             }
             else
             {

 //                qDebug() << "set right ip and port22";
                 sprintf_s(KeyData, 500, "%s%s%s%d", ProcessName.toStdString().c_str(), proxydata[1].toLocal8Bit().data(), "K120R20K21e4yF0", GetCurrentThreadId() * 5 + 12242022);
                 if (SetProxyInfo_(ProxyTypes, DNSSwitch, (char *)DNSIP.toStdString().c_str(), DNSPort,
                                   (char *)ProcessName.toStdString().c_str(), vecalldata[i].pid,
                                   (char *)proxydata[1].toLocal8Bit().data(),
                                   atoi(proxydata[2].toLocal8Bit().data()), proxydata[3].toLocal8Bit().data(),
                                   proxydata[4].toLocal8Bit().data(), MD5Data(KeyData)))
                 {

                    qDebug("SetProxyInfo_ succeed");

 //                    qDebug("SetProxyInfo_ succeed name=%s  pid=%d ip=%s port=%s username=%s passwd=%s ", ProcessName.toStdString().c_str(),
 //                           vecalldata[i].pid, (char *)proxydata[1].toLocal8Bit().data(), proxydata[2].toLocal8Bit().data(),
 //                            proxydata[3].toLocal8Bit().data(), proxydata[4].toLocal8Bit().data());

                 }
                 else
                 {

                     qDebug("SetProxyInfo_ failure");
                 }
             }
         }
    }

    fileread.close();
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    for (auto data:veccap)
    {
        for (int i=0; i<2; i++)
        {
            ProxyFuncThd * m_cpyThread;
            m_cpyThread = new ProxyFuncThd(data, i, this);
    //        m_cpyThread->SetData(veccap);
            m_cpyThread->start();
        }

    }

    return strJson;
}

QString CFunction::isopenproxy()
{
    return settings->value("functionsetting/openproxy").toString();
}

 QString CFunction::setConfInfo(QString conf)
 {
     settings->setIniCodec(QTextCodec::codecForName("gbk"));
//     qDebug() << "conf111=" << conf;
    QStringList oneline=conf.split("|");
    settings->setValue("Identificationmethod/processname", oneline[0]);
    settings->setValue("Identificationmethod/path", oneline[1]);
    settings->setValue("Identificationmethod/processMD5", oneline[2]);

    settings->setValue("functionsetting/isbannetwork", oneline[3]);
    settings->setValue("functionsetting/issubsameaddr", oneline[4]);
    settings->setValue("functionsetting/iscapture", oneline[5]);
    settings->setValue("functionsetting/packetvitual", oneline[6]);

    settings->setValue("functionsetting/issubsamehardaddr", oneline[7]);
    settings->setValue("functionsetting/issuspendprocess", oneline[8]);
    QString resdata = oneline[10] + ":"+oneline[9];
    settings->setValue("functionsetting/processnoflowtime", resdata);
    settings->setValue("functionsetting/isstart", oneline[11]);


    settings->setValue("functionsetting/ismin", oneline[12]);
    settings->setValue("functionsetting/istrap", oneline[13]);

    settings->setValue("functionsetting/openproxy", oneline[18]);
    settings->setValue("functionsetting/firewallprohibits", oneline[19]);
    settings->setValue("functionsetting/vmmac", oneline[20]);
    settings->setValue("functionsetting/vmdiskserial", oneline[21]);
    settings->setValue("functionsetting/vmdvolumelabel", oneline[22]);
    settings->setValue("functionsetting/vmdRegistryFeatures", oneline[23]);
    settings->setValue("functionsetting/vmProtectionIsolation", oneline[24]);

    settings->setValue("functionsetting/proxychildproc", oneline[25]);
    settings->setValue("functionsetting/virtualhardinfo", oneline[26]);

    settings->setValue("mode/parsetype", oneline[15]);
    settings->setValue("mode/dnsip", oneline[16]);
    settings->setValue("mode/port", oneline[17]);

    QStringList codecForNameList;
    codecForNameList<<"utf-8"<<"gbk";//几种格式轮流读，没有非法字符就对了

    QString strtmp = oneline[14];
//    qDebug() << "11111 str=" << strtmp;
//    QTextCodec *utf8 = QTextCodec::codecForName("gbk");
//    QString strpos = oneline[7];
//    strtmp = utf8->toUnicode(strtmp.toStdString().c_str());
    settings->setValue("functionsetting/title", strtmp);
     return "";
 }

 QString CFunction::editproc(QString procinfo)
 {
     qDebug() << "procinf=" << procinfo;
     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\procinfo.txt";

     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly))
       qDebug()<<"File open failed";
     QTextStream in(&fileread);
     QString dev;
     QString strres = "";

     QStringList strdata=procinfo.split("|");
    int srcid = strdata[0].toInt();

     while(!in.atEnd())
     {
       QJsonObject pageObject;
       QString lines=in.readLine();
       QString strtmp = lines;
       QStringList oneline=lines.split("|");
       int resid = atoi(oneline[0].toLocal8Bit().data());
       qDebug() << "srcid=" << srcid << " resid=" << resid;
       if (resid == srcid)
       {
           qDebug() << "editproc i find it lines=" << lines;
//           qDebug() << "status=" << oneline[13].toLocal8Bit().data();
           QString str = strdata[10];
           str.replace("\n", "-");
            QString strline = oneline[0]+"|"+strdata[1] + "|" + strdata[2] + "|" + strdata[3] + "|" + "0" + "|"
                  + strdata[4] + "|" + strdata[5] + "|"
                  + strdata[6] + "|" + strdata[7] + "|"
                  + strdata[8]+ "|" + strdata[9]+ "|" + str
                    + "|" + strdata[11]+ "|" + strdata[12]
                    + "|" + strdata[13];
//             qDebug() << "id=" << resid << " can find";
             strres=strres+strline+"\n";
       }
       else
       {
         strres=strres+strtmp+"\n";
       }
     }

     fileread.close();
     QFile filewrite(strfillpath);
     if(!filewrite.open(QIODevice::WriteOnly))
     {
        qDebug()<<"File open failed";
        return "修改成功0, 失败1";
     }

     QTextStream out(&filewrite);
     filewrite.write(strres.toStdString().data());//c_str
     filewrite.close();
     return "修改成功1";
 }

 QString CFunction::sethighlevelsetting(QString conf)
 {
//     settings->setIniCodec(QTextCodec::codecForName("gbk"));
    qDebug() << "conf=" << conf;
    QStringList oneline=conf.split("|");
    highlevelsettings->setValue("global/setprocpri", oneline[0]);
    highlevelsettings->setValue("generate/detquerythd", oneline[1]);
    highlevelsettings->setValue("generate/timeout", oneline[2]);
    highlevelsettings->setValue("generate/latencydettimeout", oneline[3]);
    highlevelsettings->setValue("generate/bindwindowtimeout", oneline[4]);
    highlevelsettings->setValue("generate/chkproxyavail", oneline[5]);
    highlevelsettings->setValue("generate/deccycle", oneline[6]);


    highlevelsettings->setValue("mode/driverkernelmode", oneline[7]);
    highlevelsettings->setValue("mode/generatemode", oneline[8]);
    highlevelsettings->setValue("mode/driverprocessmode", oneline[9]);
    highlevelsettings->setValue("mode/commonprocessmode", oneline[10]);
     return "";
 }

 QVector <QString> CFunction::readProxyConf()
 {
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
//    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
     qDebug()<<"File open failed";
    QTextStream in(&fileread);

    QString dev;
    QVector <QString> vecStr;
    while(!in.atEnd())
    {
        QJsonObject pageObject;
        QString lines=in.readLine();
        vecStr.push_back(lines);
    }

    fileread.close();
    return vecStr;
 }

 int CFunction::writeProxyConf(QVector <QString> &vecdata)
 {
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";

    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return -1;
    }

    QTextCodec *codecUTF8 = QTextCodec::codecForName("UTF-8");

    QTextStream out(&filewrite);
    QString strres = "";
    QString strtmp = "";
    QTextCodec::ConverterState state;
    QTextCodec::ConverterState state1;
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    for (auto lines:vecdata)
    {
        QStringList oneline=lines.split("|");
        int resid = atoi(oneline[0].toLocal8Bit().data());
        QString province1 = codecUTF8->toUnicode( oneline[7].toLocal8Bit().data(), oneline[7].toLocal8Bit().size(), &state);
        QString line1 = codecUTF8->toUnicode( oneline[8].toLocal8Bit().data(), oneline[8].toLocal8Bit().size(), &state1);

        province1 = gbk->toUnicode(province1.toLocal8Bit().data(), province1.toLocal8Bit().size(), &state);
        line1 = gbk->toUnicode(line1.toLocal8Bit().data(), line1.toLocal8Bit().size(), &state1);

        int cnt = oneline.count();
        strtmp = "";
        for (int i=0; i<cnt; i++)
        {
            if (7 == i)
                strtmp += province1;
            else if (8 == i)
                strtmp += line1;
            else
                strtmp += oneline[i];
            if (i !=  cnt - 1)
                strtmp += "|";
        }

//        qDebug() << "strtmp=" << strtmp;

        strres=strres+strtmp+"\n";

    }

    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    return 0;
 }

 typedef struct _configuration Configuration;
  struct _configuration
  {
    int port;
  };

QVector <captureinfo>  ProxyFuncThd::m_veccap;

 ProxyFuncThd::ProxyFuncThd(captureinfo &capinfo, int type, CFunction *func, QObject * parent)
 {
     m_type = type;
     m_func = func;
     ProxyFuncThd::lasttime = time(NULL);
    ProxyFuncThd::m_veccap.push_back(capinfo);
    Sleep(400);
 }

#define MAX_PRINT 80
#define MAX_LINE 16

struct ip_header
{
#if defined(WORDS_BIENDIAN)
    u_int8_t   ip_version : 4,
    ip_header_length : 4;
#else
    u_int8_t   ip_header_length : 4,
    ip_version : 4;
#endif
    u_int8_t    ip_tos;
    u_int16_t   ip_length;
    u_int16_t   ip_id;
    u_int16_t   ip_off;
    u_int8_t    ip_ttl;
    u_int8_t    ip_protocol;
    u_int16_t   ip_checksum;
    struct in_addr ip_souce_address;
    struct in_addr ip_destination_address;
};

struct ether_header
{
    u_int8_t ether_dhost[6]; //目的Mac地址
    u_int8_t ether_shost[6]; //源Mac地址
    u_int16_t ether_type;    //协议类型
};

unsigned long ProxyFuncThd::totalsnddata = 0;
unsigned long ProxyFuncThd::totalrecvdata = 0;
time_t ProxyFuncThd::lasttime = 0;

void ProxyFuncThd::ip_protool_packet_callback(u_char *argument, const struct pcap_pkthdr* packet_header, const u_char* packet_content)
{
    struct ip_header *ip_protocol;
    u_int header_length;
    u_int offset;
    u_char tos;
    u_int16_t checksum;
    //MAC首部是14位的，加上14位得到IP协议首部
    ip_protocol = (struct ip_header *) (packet_content + 14);
    checksum = ntohs(ip_protocol->ip_checksum);
    tos = ip_protocol->ip_tos;
    offset = ntohs(ip_protocol->ip_off);
    switch (ip_protocol->ip_protocol)
    {
//    case 1: printf("ICMP\n"); break;
//    case 2: printf("IGMP\n"); break;
    case 6:
    {
//        printf("TCP11\n");
        tcp_protocol_packet_callback(argument,packet_header,packet_content);
        break;
    }

//    case 17: printf("UDP\n"); break;
    default:break;
    }

}



 /* 对每一个到来的数据包调用该函数 */
 void ProxyFuncThd::packet_handler(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content)
 {
     struct tm *ltime;
     char timestr[16];
    time_t sec = packet_header->ts.tv_sec;

     /* 将时间戳转变为易读的标准格式*/
    ltime=localtime(&sec);
     strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
         u_short ethernet_type;
         struct ether_header *ethernet_protocol;
         u_char *mac_string;
         static int packet_number = 1;
//         printf("----------------------------------------------\n");
//         printf("packet num %d\n", packet_number);
//         printf("len:\n");
//         printf("%d\n", packet_header->caplen);
         Configuration *conf = (Configuration *) argument;
//         printf ("test: %d\n", conf[0].port);

//        printf("argument=%s\n", argument);
//         totalsnddata += packet_header->caplen;
//         totalrecvdata += packet_header->caplen;

//        return;
         ethernet_protocol = (struct ether_header*)packet_content;//获得数据包内容
//         printf("type:\n");
         ethernet_type = ntohs(ethernet_protocol->ether_type);//获得以太网类型

         mac_string = ethernet_protocol->ether_shost;
//         printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac_string, *(mac_string + 1), *(mac_string + 2), *(mac_string + 3), *(mac_string + 4), *(mac_string + 5));
//         printf("MAC dest:\n");
         mac_string = ethernet_protocol->ether_dhost;
//         printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac_string, *(mac_string + 1), *(mac_string + 2), *(mac_string + 3), *(mac_string + 4), *(mac_string + 5));
         if (ethernet_type == 0x0800)//继续分析IP协议
         {
             ip_protool_packet_callback(argument, packet_header, packet_content);
         }
 }

 NetworkPerformanceScanner::NetworkPerformanceScanner()
 {

 }


 NetworkPerformanceScanner::~NetworkPerformanceScanner()
 {
 }

 // TODO - implement TCP v6, UDP
 std::vector<NetworkPerformanceItem> NetworkPerformanceScanner::ScanNetworkPerformance(int pass, int port)
 {

     std::vector<unsigned char> buffer;
     DWORD dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
     DWORD dwRetValue = 0;
     vector<NetworkPerformanceItem> networkPerformanceItems;

     // get local computer time with timezone offset
     auto time = std::time(nullptr);
     std::ostringstream timeStream;
     timeStream << std::put_time(std::localtime(&time), "%F %T%z");
     string collectionTime = timeStream.str();

     // repeat till buffer is big enough
     do
     {
         buffer.resize(dwSize, 0);
         dwRetValue = GetExtendedTcpTable(buffer.data(), &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0);

     } while (dwRetValue == ERROR_INSUFFICIENT_BUFFER);

     if (dwRetValue == ERROR_SUCCESS)
     {
         // good case

         // cast to access element values
         PMIB_TCPTABLE_OWNER_PID ptTable = reinterpret_cast<PMIB_TCPTABLE_OWNER_PID>(buffer.data());

//         cout << "Number of Entries: " << ptTable->dwNumEntries << endl << endl;


         // caution: array starts with index 0, count starts by 1
         for (DWORD i = 0; i < ptTable->dwNumEntries; i++)
         {
             NetworkPerformanceItem networkPerformanceItem;

             networkPerformanceItem.ProcessId = ptTable->table[i].dwOwningPid;
             networkPerformanceItem.State = ptTable->table[i].dwState;
            if (pass != ptTable->table[i].dwOwningPid)
            {
//                cout << "PID: " << ptTable->table[i].dwOwningPid << endl;
//                cout << "State: " << ptTable->table[i].dwState << endl;
                continue;
            }


             std::ostringstream localStream;
             localStream << (ptTable->table[i].dwLocalAddr & 0xFF)
                 << "."
                 << ((ptTable->table[i].dwLocalAddr >> 8) & 0xFF)
                 << "."
                 << ((ptTable->table[i].dwLocalAddr >> 16) & 0xFF)
                 << "."
                 << ((ptTable->table[i].dwLocalAddr >> 24) & 0xFF)
                 << ":"
                 << htons((unsigned short)ptTable->table[i].dwLocalPort);
             networkPerformanceItem.LocalAddress = localStream.str();
             networkPerformanceItem.LocalPort = ptTable->table[i].dwLocalPort;

             std::ostringstream remoteStream;
             remoteStream << (ptTable->table[i].dwRemoteAddr & 0xFF)
                 << "."
                 << ((ptTable->table[i].dwRemoteAddr >> 8) & 0xFF)
                 << "."
                 << ((ptTable->table[i].dwRemoteAddr >> 16) & 0xFF)
                 << "."
                 << ((ptTable->table[i].dwRemoteAddr >> 24) & 0xFF)
                 << ":"
                 << htons((unsigned short)ptTable->table[i].dwRemotePort);
             networkPerformanceItem.RemoteAddress = remoteStream.str();
//              qDebug() << "port=" << port;
             networkPerformanceItem.RemotePort = ptTable->table[i].dwRemotePort;
//            if (port!= ptTable->table[i].dwLocalPort
//                    || ptTable->table[i].dwLocalPort != ptTable->table[i].dwRemotePort)
//            {
//                continue;
//            }
//             qDebug() << "LocalAddress=" << localStream.str().c_str()
//             << " LocalPort=" << ptTable->table[i].dwLocalPort
//                     << " RemoteAddress=" << remoteStream.str().c_str()
//                     << " RemotePort=" << ptTable->table[i].dwRemotePort;

             MIB_TCPROW row;
             row.dwLocalAddr = ptTable->table[i].dwLocalAddr;
             row.dwLocalPort = ptTable->table[i].dwLocalPort;
             row.dwRemoteAddr = ptTable->table[i].dwRemoteAddr;
             row.dwRemotePort = ptTable->table[i].dwRemotePort;
             row.dwState = ptTable->table[i].dwState;
             void *processRow = &row;

             if (row.dwRemoteAddr != 0)
             {
                 ULONG rosSize = 0, rodSize = 0;
                 ULONG winStatus;
                 PUCHAR ros = NULL, rod = NULL;
                 rodSize = sizeof(TCP_ESTATS_DATA_ROD_v0);
                 PTCP_ESTATS_DATA_ROD_v0 dataRod = { 0 };

                 if (rosSize != 0) {
                     ros = (PUCHAR)malloc(rosSize);
                     if (ros == NULL) {
//                         wprintf(L"\nOut of memory");
                         return networkPerformanceItems;
                     }
                     else
                         memset(ros, 0, rosSize); // zero the buffer
                 }
                 if (rodSize != 0) {
                     rod = (PUCHAR)malloc(rodSize);
                     if (rod == NULL) {
                         free(ros);
//                         wprintf(L"\nOut of memory");
                         return networkPerformanceItems;
                     }
                     else
                         memset(rod, 0, rodSize); // zero the buffer
                 }

                 winStatus = GetPerTcpConnectionEStats((PMIB_TCPROW)&row, TcpConnectionEstatsData, NULL, 0, 0, ros, 0, rosSize, rod, 0, rodSize);

                 dataRod = (PTCP_ESTATS_DATA_ROD_v0)rod;

                 networkPerformanceItem.BytesIn = dataRod->DataBytesIn;
                 networkPerformanceItem.BytesOut = dataRod->DataBytesOut;
                qDebug() << "BytesIn: " << networkPerformanceItem.BytesIn;
                qDebug() << "BytesOut: " << networkPerformanceItem.BytesOut;
                 PTCP_ESTATS_BANDWIDTH_ROD_v0 bandwidthRod = { 0 };

                 rodSize = sizeof(TCP_ESTATS_BANDWIDTH_ROD_v0);
                 if (rodSize != 0) {
                     rod = (PUCHAR)malloc(rodSize);
                     if (rod == NULL) {
                         free(ros);
//                         wprintf(L"\nOut of memory");
                         return networkPerformanceItems;
                     }
                     else
                         memset(rod, 0, rodSize); // zero the buffer
                 }

                 winStatus = GetPerTcpConnectionEStats((PMIB_TCPROW)&row,TcpConnectionEstatsBandwidth, NULL, 0, 0, ros, 0, rosSize, rod, 0, rodSize);

                 bandwidthRod = (PTCP_ESTATS_BANDWIDTH_ROD_v0)rod;
                 networkPerformanceItem.OutboundBandwidth = bandwidthRod->OutboundBandwidth;
                 networkPerformanceItem.InboundBandwidth = bandwidthRod->InboundBandwidth;

             }
             networkPerformanceItem.Pass = pass;
             networkPerformanceItem.CollectionTime = collectionTime;
             networkPerformanceItems.push_back(networkPerformanceItem);
         }
     }
     else
     {
         // bad case, do some sh*t here
     }

     return networkPerformanceItems;
 }

 /*下边实现tcp数据包分析的函数定义tcp_protocol_packet_callback*/
 void ProxyFuncThd::tcp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr *packet_header,const u_char* packet_content)
 {

      struct tcp_headerinfo *tcp_protocol=(struct tcp_headerinfo *) (packet_content+14+20);  /*获得tcp首部内容*/
      unsigned short tcp_source_port = tcp_protocol->tcp_source_port;
//      qDebug("tcp_source_port port= %d\n",ntohs(tcp_source_port));

        unsigned short tcp_destination_port = tcp_protocol->tcp_destination_port;
//      qDebug("tcp_destination_port port= %d\n",ntohs(tcp_destination_port));

      Configuration *conf = (Configuration *) argument;
//      printf ("test: %d\n", conf[0].port);


      unsigned short checksum =ntohs (tcp_protocol->tcp_checksum);

       if (conf[0].port == ntohs(tcp_source_port))
       {
//           qDebug("tcp_destination_port port= %d\n",ntohs(tcp_destination_port));
           string strtmp = to_string(ntohs(tcp_destination_port));
            string strpid = to_string(ProxyFuncThd::m_veccap[0].pid);
            getpidbyport((char *)strtmp.c_str(), (char *)strpid.c_str());
//            getpidbyport((char *)strtmp.c_str());
           totalrecvdata += packet_header->caplen;
       }
       else
       {
//           qDebug("tcp_source_port port= %d\n",ntohs(tcp_source_port));
           string strtmp = to_string(ntohs(tcp_source_port));
           string strpid = to_string(ProxyFuncThd::m_veccap[0].pid);
           getpidbyport((char *)strtmp.c_str(), (char *)strpid.c_str());
         totalsnddata += packet_header->caplen;
       }
//      qDebug() << "checksum=" << checksum;
 }

CFunction *ProxyFuncThd::m_func;



 void ProxyFuncThd::run()
 {
     if (0 == m_type)
     {
         pcap_if_t *alldevs;
         pcap_if_t *d;
//         int i=0;
         pcap_t *adhandle;//定义文件句柄
         char errbuf[PCAP_ERRBUF_SIZE];

         string maca;

         std::string name;
         u_int netmask; //定义子网掩码
         struct bpf_program fcode;
         GetMacByGetAdaptersInfo(maca, name);

         /* 获得驱动列表 */
         if (pcap_findalldevs(&alldevs, errbuf) == -1)
         {
             fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
             exit(1);
         }

         for(d=alldevs; d; d=d->next)
         {
//             qDebug("[%s] [%s]",
//                    d->name, d->description);
         }

         /* 打印 list */
         string strname;
         for(d=alldevs; d; d=d->next)
         {
             if (d->addresses&& d->addresses->addr && strstr(d->name, name.c_str()) != NULL)
             {
//                 qDebug("data=[%d] [%s] [%s] [%s] [%s]",
//                      ++i, name.c_str(), iptos(((struct sockaddr_in *)d->addresses->addr)->sin_addr.s_addr),
//                        d->name, d->description);
                 break;
             }

         }

         if ( (adhandle= pcap_open_live(d->name,65536, 1,1000,errbuf) ) == NULL)
          {
              fprintf(stderr,"\nopen failure. %s not support\n",d->name);
              /* 释放设备列表 */
              pcap_freealldevs(alldevs);
//               qDebug("reCapture: 1");
              return;
          }
         else
         {
            fprintf(stderr,"\ncapture succeed\n",d->name);
         }

        if (d->addresses != NULL)
        {
           // 获得接口的第一个地址的掩码
           netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
        //                  netmask=((sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
        }
        else
        {
            netmask = 0xffffff;
        }



        QString filterinfo = QString::asprintf("src port %d or dst port %d", ProxyFuncThd::m_veccap[0].port, ProxyFuncThd::m_veccap[0].port);
//        qDebug() << "filterinfo=" << filterinfo;
        if(pcap_compile(adhandle,&fcode,filterinfo.toStdString().c_str(),1,netmask)<0)
         {
             qDebug("\nUnable to compile the packet filter.Check the syntax.\n");
             pcap_freealldevs(alldevs);

         }


         int rett = pcap_setfilter(adhandle,&fcode);
         if(rett < 0)
         {
             qDebug() << "Error setting 111 the filter tmpdata=" << rett;
             pcap_freealldevs(alldevs);
         }

      /* 开始捕获包 */
         Configuration conf;
         conf.port = ProxyFuncThd::m_veccap[0].port;
      pcap_loop(adhandle, 0, packet_handler, (u_char *)&conf);

      qDebug() << "run next***********************" << endl;
     }
     else
     {
         time_t begintime = time(NULL);

         int hour=0,min=0,sec=0;
        while (1)
        {
            Sleep(1000);
            time_t endtime = time(NULL);
            time_t diff = endtime - begintime;
            hour = diff / 3600;
            min = (diff-hour * 3600) / 60;
            sec = diff-hour * 3600-min * 60;
            char buf[64];

    //            QString strtotalsnddata = QString::number(totalsnddata);
    //            QString strtotalrecvdata = QString::number(totalrecvdata);


//            NetworkPerformanceScanner scanner;
//            vector<NetworkPerformanceItem> vecres = scanner.ScanNetworkPerformance(m_veccap[0].pid,
//                    m_veccap[0].port);
//            if (vecres.size() >0)
//            {
//                QString strport = QString::number(m_veccap[0].port);
//                 int conninfo = getconnnum((char *)strport.toStdString().c_str());
//                 QString strconninfo = QString::number(conninfo);
//                snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour, min, sec);
//                QString strid = QString::number(m_veccap[0].id);
//    //            qDebug() << "strid=" << strid;


//                totalrecvdata += vecres[0].BytesIn;
////                qDebug("out=%d", vecres[0].BytesOut);
//                totalsnddata += vecres[0].BytesOut;


//                double sndspeed = totalsnddata;
//                double recvspeed = totalrecvdata;

//                QString strtotalsnddata = QString::number(sndspeed,'f',1) + "B";
//                QString strtotalrecvdata = QString::number(recvspeed,'f',1) + "B";

//                if (sndspeed > 1024*1024)
//                {

//                    sndspeed /= 1024*1024;
//                     //保留几位1就改成几
//                     strtotalsnddata = QString::number(sndspeed,'f',1) + "Mb";

//                }
//                else if (sndspeed > 1024)
//                {

//                    sndspeed /= 1024;
//                     strtotalsnddata = QString::number(sndspeed,'f',1) +  "kb";

//                }
//                else
//                {
//    //                sndspeed /=1024*1024;
//                }

//                if (recvspeed > 1024*1024)
//                {
//                    recvspeed /= 1024*1024;

//                    strtotalrecvdata = QString::number(recvspeed,'f',1) + "Mb";
//                }
//                else if (recvspeed > 1024)
//                {

//                    recvspeed /= 1024;

//                    strtotalrecvdata = QString::number(recvspeed,'f',1) + "kb";
//                }
//                else
//                {

//                }

//                emit ProxyFuncThd::m_func->updateproxyinfo(strid, buf, strtotalsnddata, strtotalrecvdata, strconninfo);	// 发射信号
//            }

//            std::vector<unsigned char> buffer;
//            DWORD dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
//           DWORD dwBufferSize = 0;
//           GetExtendedTcpTable(NULL, &dwBufferSize, true, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
//           /*创建缓冲区*/
//       //    MIB_TCPTABLE_OWNER_PID mibTcpTable;// = (MIB_TCPTABLE_OWNER_PID*)malloc(dwBufferSize);
//           buffer.resize(dwSize, 0);
//           /*取Tcp Table*/
//           DWORD dwRet = GetExtendedTcpTable(buffer.data(), &dwBufferSize, true, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
//           if(dwRet != NO_ERROR)
//           {
//               qDebug("GetExtendedTcpTable != ERROR_SUCCESS, ErrorCode=%d\n", dwRet);
//       //        free(pMibTcpTable);
//               return;
//           }
//           else
//           {
//               qDebug("ERROR_SUCCESS\n");
//           }

//           qDebug("process next");

//           PMIB_TCPTABLE_OWNER_PID pMibTcpTable = reinterpret_cast<PMIB_TCPTABLE_OWNER_PID>(buffer.data());
//           for(DWORD i = 0; i != pMibTcpTable->dwNumEntries; ++i)
//               {
//                   /*获取端口状态*/
//       //            DWORD dwState = pMibTcpTable->table[i].dwState;

//                   /*生成IP*/
//                   in_addr inAddrLocal, inAddrRemote;
//                   memcpy(&inAddrLocal, &(pMibTcpTable->table[i].dwLocalAddr), sizeof(DWORD));
//                   memcpy(&inAddrRemote, &(pMibTcpTable->table[i].dwRemoteAddr), sizeof(DWORD));
//                   std::string sIpLocal = inet_ntoa(inAddrLocal);
//                   std::string sIpRemote = inet_ntoa(inAddrRemote);

//                   /*获取Port*/
//                   DWORD dwPortLocal = ntohs(pMibTcpTable->table[i].dwLocalPort);
//                   DWORD dwPortRemote = ntohs(pMibTcpTable->table[i].dwRemotePort);

//                   /*获取PID*/
//                   DWORD dwPid = pMibTcpTable->table[i].dwOwningPid;

//                   printf("端口状态=%d, 本地地址=%s, 本地端口=%d, 远端地址=%s, 远端端口=%d, 进程号=%d\n",
//                       pMibTcpTable->table[i].dwState, sIpLocal.c_str(), dwPortLocal, sIpRemote.c_str(), dwPortRemote, dwPid);
//               }
//            Sleep(1000);
//            emit ProxyFuncThd::m_func->updatetimeinfo(buf);
        }
     }
 }

 ProcFuncThd::ProcFuncThd(int type, CFunction *func, QObject * parent)
 {
     ProcFuncThd::m_func = func;
     m_type = type;
 }

 QVector <ProcData> readProcConf()
 {
     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\procinfo.txt";
//     qDebug() << "strfillpath=" << strfillpath;
     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
         qDebug()<<"File open failed";
     QTextStream in(&fileread);

     int idx = 0;
     int id = 0;
     QString dev;
     QVector <ProcData> vecprocname;
     ProcData procdata;
     while(!in.atEnd())
     {
         QJsonObject pageObject;
         QString lines=in.readLine();

         QStringList oneline=lines.split("|");
         procdata.strprocname = oneline[1].trimmed().toLocal8Bit().data();
         vecprocname.push_back(procdata);
     }

     fileread.close();

     return vecprocname;
 }

 QString addCurProcInfo(QVector <CaptureData> &vecdata)
 {
     QString strfillpath = path;
     strfillpath += "\\curprocinfo.txt";
     int idx = 1;

     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
     {
         qDebug()<<"File open failed";
     }
     else
     {
         QFile fileread(strfillpath);
         if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
             qDebug()<<"File open failed";
         QTextStream in(&fileread);
         QString lines = "";
         while(!in.atEnd())
         {
             lines=in.readLine();
         }

//         qDebug() << "lines111=" << lines;
         if (lines != "")
         {
             QStringList strlist=lines.split("|");
             if (strlist.size() > 0)
             {
                 idx = atoi(strlist[0].toUtf8().data());

                 idx++;
                  qDebug() << "idx11221=" << idx;
             }

         }

     }

     fileread.close();
     QFile file(strfillpath);

     bool isok=file.open(QIODevice::ReadWrite | QIODevice::Append);//
     if(!isok)
     {
         qDebug() << "write error";
     }

     QString strtmp;
    for (auto data:vecdata)
    {
        data.pid+data.procname + "|" + data.connection + "|" + data.sendnum + "|" + data.recvnum + "|" + data.proxyip + "|" + data.strmac + "|" + data.strpos;
        QString stridx = QString::number(idx++);
        QString strwrite = stridx + "|" + strtmp;
        file.write(strwrite.toStdString().data());//c_str
    }

     file.close();
     return "";
 }

 void __stdcall R0MonitorProcessCallback(BOOLEAN CreateTag, DWORD CurProcessID, char * CurProcessName, char * CurProcessPath, DWORD ParentPID)  //监视进程信息
 {
 //    CString Str = "";

     QVector <CaptureData> vecStr;
     QVector <ProcData> vecdata = readProcConf();
     for (auto data:vecdata)
     {
        if (0 == strcmp(CurProcessName, data.strprocname.toStdString().c_str()))
        {
                QString strresdata = ProcFuncThd::m_func->reCapture(1, CurProcessName);
                emit ProcFuncThd::m_func->addproxyinfo(strresdata);
            qDebug() << "run: PID: " << CurProcessID << " parent PID: " << ParentPID << "  current process name: " << CurProcessName <<
                        "  路径: " << CurProcessPath;
            qDebug() << "strresdata=" << strresdata;

        }
        else
        {
//            qDebug() << "cant find run: PID: " << CurProcessID << " parent PID: " << ParentPID << "  current process name: " << CurProcessName <<
//                        "  路径: " << CurProcessPath;
        }
     }

     addCurProcInfo(vecStr);

 }

 CFunction *ProcFuncThd::m_func = NULL;

 QVector <ProcData> ProcFuncThd::readProcConf()
 {
     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\procinfo.txt";
//     qDebug() << "strfillpath=" << strfillpath;
     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
         qDebug()<<"File open failed";
     QTextStream in(&fileread);
     QString dev;
     QVector <ProcData> vecprocname;
     ProcData procdata;
     while(!in.atEnd())
     {
         QJsonObject pageObject;
         QString lines=in.readLine();

         QStringList oneline=lines.split("|");
         procdata.strprocname = oneline[1].trimmed().toLocal8Bit().data();
         vecprocname.push_back(procdata);
     }

     fileread.close();
     return vecprocname;
 }

 void ProcFuncThd::run()
 {
     QString driverprocessmode = ProcFuncThd::m_func->highlevelsettings->value("mode/driverprocessmode").toString();
//     qDebug("ProcFuncThd::run 2");
     QString commonprocessmode = ProcFuncThd::m_func->highlevelsettings->value("mode/commonprocessmode").toString();
     if (driverprocessmode == "1")
     {
//         qDebug("ProcFuncThd::run 4");
         GetR0ProcessInfo_(TRUE, TRUE, R0MonitorProcessCallback, (char *)"www.8u18.com");  //监视进程信息
     }
     else
     {
         while (1)
         {
             ProcFuncThd::m_func->reCapture(0);
             sleep(2);
         }
     }

 }



#define WORLD_ACCESS GENERIC_ALL//GENERIC_READ|GENERIC_WRITE
PSID CreateWorldSid()
{
    SID_IDENTIFIER_AUTHORITY authWorld = SECURITY_WORLD_SID_AUTHORITY;
    PSID pSid, psidWorld;
    BOOL bRes;
    DWORD cbSid;

    bRes = AllocateAndInitializeSid(&authWorld,
    1,
    SECURITY_WORLD_RID,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &psidWorld);
    if (FALSE == bRes) {
    return NULL;
    }

    // Make a copy of the SID using a HeapAlloc'd block for return
    //
    cbSid = GetLengthSid(psidWorld);

    pSid = (PSID) HeapAlloc(GetProcessHeap(), 0, cbSid);
    if (NULL == pSid) {
    return NULL;
    }

    bRes = CopySid(cbSid, pSid, psidWorld);
    if (FALSE == bRes) {
    return NULL;
    }

    FreeSid(psidWorld);

    return pSid;
    }

HRESULT CFunction::SetFileRight(char* pFileObject)
{
    HRESULT hr=S_OK;
    PSID psidEveryone;
    PACL pAcl;
    SECURITY_DESCRIPTOR sd;
    BOOL bRes;
    DWORD cbAcl;
    psidEveryone = CreateWorldSid();
    cbAcl = GetLengthSid (psidEveryone) +
    sizeof(ACL) + ((sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)));
    pAcl = (PACL) HeapAlloc(GetProcessHeap(), 0, cbAcl);
    if (NULL == pAcl) {
    qDebug("SetFileRight 1");
    HeapFree(GetProcessHeap(), 0, psidEveryone);
    return S_FALSE;
    }


    bRes = InitializeAcl(pAcl,
    cbAcl,
    ACL_REVISION);
    if (FALSE == bRes) {
        qDebug("SetFileRight 2");
    hr=S_FALSE;
    goto CleanUp;
    }


    bRes = AddAccessAllowedAce(pAcl,
    ACL_REVISION,
    WORLD_ACCESS,
    psidEveryone);
    if (FALSE == bRes) {
        qDebug("SetFileRight 3");
    hr=S_FALSE;
    goto CleanUp;
    }

    // Put together the security descriptor
    //
    bRes = InitializeSecurityDescriptor(&sd,
    SECURITY_DESCRIPTOR_REVISION);
    if (FALSE == bRes) {
        qDebug("SetFileRight 4");
    hr=S_FALSE;
    goto CleanUp;
    }

    bRes = SetSecurityDescriptorDacl(&sd,
    TRUE,
    pAcl,
    FALSE);
    if (FALSE == bRes) {
        qDebug("SetFileRight 5");
    hr=S_FALSE;
    goto CleanUp;
    }

    if (!SetFileSecurity((LPCWSTR)pFileObject,
    DACL_SECURITY_INFORMATION,
    &sd))
    {
        qDebug("SetFileRight 6");
    hr=S_FALSE;

    }
    else
    hr=S_OK;
    CleanUp:
    HeapFree(GetProcessHeap(), 0, pAcl);
    HeapFree(GetProcessHeap(), 0, psidEveryone);
    return hr;
}

