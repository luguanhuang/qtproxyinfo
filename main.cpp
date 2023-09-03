#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>

#include<winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "MD5.h"
#include "log_lib_cplus.h"
#include "operproxy.h"
#include "function.h"

int DNSSwitch = 0;
InitData InitData_;
SetProxyInfo SetProxyInfo_;
StopProxy StopProxy_;
RepairNet RepairNet_;
leanData CleanData_;
heckSocks5 CheckSocks5_;
GetR0ProcessInfo GetR0ProcessInfo_;
StartVirtual StartVirtual_;
StopVirtual StopVirtual_;

#include <Iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

#ifdef UNICODE

char path[MAX_PATH];

HANDLE g_hStd_OUT_Rd = NULL;
HANDLE g_hStd_OUT_Wr = NULL;


char* getMAC()
{
    PIP_ADAPTER_INFO AdapterInfo;
    qDebug(" mac_addr 00\n");
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    char *mac_addr = (char*)malloc(18);

    AdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
    if (AdapterInfo == NULL) {
        qDebug(" mac_addr 1\n");
    qDebug("Error allocating memory needed to call GetAdaptersinfo\n");
    free(mac_addr);
    return NULL; // it is safe to call free(NULL)
    }

    // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(AdapterInfo);
    AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
    if (AdapterInfo == NULL) {
      qDebug("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
      return NULL;
    }
    }

    qDebug(" mac_addr 2\n");
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
    {
    // Contains pointer to current adapter info
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        do
        {
            // technically should look at pAdapterInfo->AddressLength
            //   and not assume it is 6.
            qDebug(" mac_addr 3\n");
            sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
            pAdapterInfo->Address[0], pAdapterInfo->Address[1],
            pAdapterInfo->Address[2], pAdapterInfo->Address[3],
            pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
            qDebug("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
            // print them all, return the last one.
            // return mac_addr;

            qDebug(" mac_addr=%s\n",  mac_addr);
            pAdapterInfo = pAdapterInfo->Next;
        } while(pAdapterInfo);
    }

    qDebug(" mac_addr 4\n");
    free(AdapterInfo);
    return mac_addr; // caller must free.
}

#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QUrl>
#include <tlhelp32.h>
#endif // !UNICODE


BOOL IsSoftRunState(DWORD PID)  //判断进程是否运行
{
    BOOL Ret = FALSE;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    __try
    {
        if (hProcessSnap != INVALID_HANDLE_VALUE)   //防止野指针
        {
            PROCESSENTRY32 pe32 = { sizeof(pe32) };
            BOOL bFind = Process32First(hProcessSnap, &pe32);

            while (bFind)
            {
                if (pe32.th32ProcessID == PID)
                {
                    Ret = TRUE;
                    break;
                }

                bFind = Process32Next(hProcessSnap, &pe32);
            }
        }
    }
    __finally
    {
        ::CloseHandle(hProcessSnap);
    }

    return Ret;
}

int my_CreateProcess1()
{
    BOOL run_pipe;

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret = FALSE;
    DWORD flags = CREATE_NO_WINDOW;

    _unlink("D:/out/output.log");
    qDebug("my_CreateProcess1: 1");
    char pBuffer[210];
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    HANDLE hReadPipe, hWritePipe;
    run_pipe=CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
//    printf("run_pipe=%d\n", run_pipe);

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;

    TCHAR cmd[] = TEXT("netstat -any");
    int cnt = 0;
    ret = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
    if (ret)
    {
        qDebug("my_CreateProcess1: 2");
//        while (true)
//        {
//            DWORD ExitCode = 0;
//            //判断进程是否执行结束
//            GetExitCodeProcess(pi.hProcess, &ExitCode);
//            qDebug("my_CreateProcess1: 3");
//            if (ExitCode == STILL_ACTIVE) //正在运行
//            {
//                qDebug("my_CreateProcess1: 4");
//                DWORD RSize=0;
//                BOOL run_s=0;
//                run_s =ReadFile(hReadPipe, pBuffer,200,&RSize,NULL);
//                pBuffer[RSize] = '\0';
//                if (strstr(pBuffer, "ESTABLISHED") != NULL)
//                    cnt++;
////                    printf("res:%d,%d,%s\n", run_s, RSize, pBuffer);
//            }
//            else //结束
//            {
//                qDebug("end...\n");
//                break;
//            }
//        }

        //WaitForSingleObject(pi.hProcess, INFINITE);
        qDebug("total=%d\n", cnt);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 0;
    }
//    printf("执行失败....\n");
    return -1;
}

int getip()
{
    struct hostent* phost = gethostbyname("jsxz18.dxycip.top");
    char* ip;
    if (NULL == phost)
    {
        cout << "gethostbyname error : " <<  errno << " : " << strerror(errno) << endl;
        return -1;
    }
    //inet_ntop(phost->h_addrtype,  phost->h_addr, ip, 17);
    for(int i=0; phost->h_addr_list[i]; i++) {
        ip = inet_ntoa( *(struct in_addr*)phost->h_addr_list[0] );
        cout << ip << endl;
    }

    return 0;
}

int main(int argc, char *argv[])
{

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gb2312"));

//   my_CreateProcess();
//    fopen();
//        return 0;




    WSADATA wsaData;
    int err;
    WORD wVersionRequested;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        return 0;
    }

//     logDebug(log, "main 111");

    if (LOBYTE(wsaData.wVersion) != 1 ||
        HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();
        return 0;
    }

//     return getip();

    memset(path, 0, sizeof(path));
    GetModuleFileNameA(NULL, path, MAX_PATH);
    *strrchr(path, '\\') = '\0';                    // 截取路径，如：E:\Tools
    char resdata[256];
    snprintf(resdata, sizeof(resdata), "%s\\server", path);

    string str = resdata;
    str += "/";
    str+="server";
    CLog log(str.c_str());
    if(log.init(CLOG_DEBUG) < 0)
    {
        fprintf(stderr, "init log faild.\n");
    }

//    QTextCodec *codec = QTextCodec::codecForName("utf-8");

//       QTextCodec::setCodecForLocale(codec);
//       QTextCodec::setCodecForCStrings(codec);
//       QTextCodec::setCodecForTr(codec);


//       QTextCodec::setCodecForLocale(codec);




#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
//    QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

//    BOOL ret = StartVirtual_((char *)"www.8u18.com");
//    char *data = getMAC();
//    qDebug() << "getMAC111=" << data << " ret=" << ret;
////    qDebug() << " ret=" << ret;
//    StopVirtual_();



//    if (!IsSoftRunState(11223))
//    {
//        qDebug("error");
////        AfxMessageBox("错误：指定的进程Pid未运行，代理失败！", MB_TOPMOST | MB_OK | MB_ICONERROR);
////        return;
//    }
//    else
//    {
//        qDebug("exist");
//    }

    QQmlApplicationEngine engine;
    COperProxy operProxy;
    engine.rootContext()->setContextProperty("operProxy", &operProxy);

    CFunction func;
    engine.rootContext()->setContextProperty("func", &func);
    engine.rootContext()->setContextProperty("appDir",app.applicationDirPath());
    const QUrl url(QStringLiteral("qrc:/main.qml"));



//    qDebug("*******************drivertype=%d", drivertype);
#if 1
    snprintf(resdata, sizeof(resdata), "%s\\JSBProxy32.dll", path);
    qDebug() << "resdata=" << resdata;
    HINSTANCE hInst = LoadLibraryA((resdata)); //载入代理DLL
    if (hInst)
    {

//        logDebug(log, "");
        RepairNet_ = (RepairNet)GetProcAddress(hInst, "RepairNet");

        InitData_ = (InitData)GetProcAddress(hInst, "InitData");
        SetProxyInfo_ = (SetProxyInfo)GetProcAddress(hInst, "SetProxyInfo");

        StopProxy_ = (StopProxy)GetProcAddress(hInst, "StopProxy");
        CleanData_ = (leanData)GetProcAddress(hInst, "CleanData");

        CheckSocks5_ = (heckSocks5)GetProcAddress(hInst, "CheckSocks5");

        GetR0ProcessInfo_ = (GetR0ProcessInfo)GetProcAddress(hInst, "GetR0ProcessInfo");

        if (!SetProxyInfo_ || !StopProxy_ || !CleanData_ || !InitData_ || !RepairNet_ || !CheckSocks5_ || !GetR0ProcessInfo_)
        {
             qDebug() << "888";
            ExitProcess(0);
        }

        string str = "LoadLibraryA JSBProxy32.dll succeed 888344";
        logDebug(log, str.c_str());
        //bool ret = InitData_(20080, 0, TRUE);  //初始化
        int drivertype = func.getdrivertype();
        bool ret = InitData_(20080, drivertype, TRUE);  //初始化

//        qDebug() << "test12222="<<ret;

        HINSTANCE m_hInst = ::LoadLibrary(TEXT("VirtualHW.dll")); //载入 DLL
        if (m_hInst)
        {
            StartVirtual_ = (StartVirtual)GetProcAddress(m_hInst, "StartVirtualHW");
            StopVirtual_ = (StopVirtual)GetProcAddress(m_hInst, "StopVirtualHW");

            if (!StartVirtual_ || !StopVirtual_)
            {

                  logDebug(log, "LoadLibraryA VirtualHW.dll failure");
             }

            else
            {
                qDebug() << "LoadLibraryA VirtualHW.dll succeed";
            }
        }
        else
        {
            logDebug(log, "LoadLibraryA  JSBProxy32.dll failure");
        }
    }
    else
    {
        qDebug() << "load failure1111";
//        logDebug(log, "load failure");
    }

    if (func.packetvitual)//打开虚拟环境
    {
//        BOOL ret = StartVirtual_((char *)"www.8u18.com");
//        char *data = getMAC();
//        qDebug() << "getMAC111=" << data << " ret=" << ret;
//    //    qDebug() << " ret=" << ret;
//        StopVirtual_();
    }

    QString strisopen = func.isopenproxy();
//    qDebug() << "strisopen=" << strisopen;
    if ("1" == strisopen)
        func.captureProc(func.iscapture);

#endif

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    WSACleanup();
    return app.exec();
}
