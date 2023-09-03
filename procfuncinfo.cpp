
#include "procfuncinfo.h"
#include "function.h"
#pragma comment (lib,"Psapi.lib")
#pragma comment(lib,"ws2_32.lib")
extern char path[MAX_PATH];


SchedulingStrategy::SchedulingStrategy(CFunction *func)
{
    m_func = func;
}

int SchedulingStrategy::GetProxyInfo( QVector <QString> &needcapdata, QVector <QString> &vecStr, QString &res)
{
    bool find = false;
    for (int i=0; i<needcapdata.size(); i++)
    {
        QStringList oneline=needcapdata[i].split("|");
//        qDebug() << "sta=" << oneline[5];
        if (oneline[5] == "0")//顺序
        {
            for (auto &data:vecStr)
            {
                 QStringList proxyoneline=data.split("|");
                 int canssigned = proxyoneline[10].toInt();
                 int alreadyssigned = proxyoneline[11].toInt();
                 QString strres;
                 find = false;
                 if (canssigned - alreadyssigned > 0)
                 {
                    res = data;
                    alreadyssigned += 1;
                    int cnt = proxyoneline.count();
                    for (int i=0; i<cnt; i++)
                    {
                        if (11 == i)
                            strres += QString::number(alreadyssigned, 10);
                        else
                            strres += proxyoneline[i];
                        if (i !=  cnt - 1)
                            strres += "|";
                    }


//                    qDebug() << "strres=" << strres;
                    data = strres;
                    find = true;
                 }

                if (find == true)
                {
                    goto FUNC_END;
//                    break;
                }
            }
        }
        else if (oneline[5] == "1")//倒叙
        {
            QVector<QString>::reverse_iterator rit;
            rit = vecStr.rbegin();
            for (; rit != vecStr.rend(); rit++)
            {
                QStringList proxyoneline=(*rit).split("|");
//                qDebug() << "strres=" << (*rit);
                int canssigned = proxyoneline[10].toInt();
                int alreadyssigned = proxyoneline[11].toInt();
                QString strres = "";
                find = false;
                if (canssigned - alreadyssigned > 0)
                {
                   res = (*rit);
                   alreadyssigned += 1;
                   int cnt = proxyoneline.count();
                   for (int i=0; i<cnt; i++)
                   {
                       if (11 == i)
                           strres += QString::number(alreadyssigned, 10);
                       else
                           strres += proxyoneline[i];
                       if (i !=  cnt - 1)
                           strres += "|";
                   }



                   *rit = strres;

                   find = true;
                }

               if (find == true)
               {
                   goto FUNC_END;
               }
            }
        }
//        else if (oneline[5] == "2")//均衡
//        {

//        }
//        else if (oneline[5] == "3")//随机
//        {
//            bool isok = false;
//            for (auto data:vecStr)
//            {
//                QStringList proxyoneline=data.split("|");
//                int canssigned = proxyoneline[10].toInt();
//                int alreadyssigned = proxyoneline[11].toInt();
//                QString strres = "";

//                if (canssigned - alreadyssigned > 0)
//                {
//                    isok = true;
//                    break;
//                }
//            }

//            if (isok == false)
//            {
//                return "";
//            }

//            int idx = 0;
//            while(1)
//            {
//                qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//                idx =qrand()%vecStr.size();
//                QStringList proxyoneline=vecStr[idx].split("|");
//                int canssigned = proxyoneline[10].toInt();
//                int alreadyssigned = proxyoneline[11].toInt();
//                QString strres = "";

//                if (canssigned - alreadyssigned > 0)
//                {
//                   alreadyssigned += 1;
//                   int cnt = proxyoneline.count();
//                   for (int i=0; i<cnt; i++)
//                   {
//                       if (11 == i)
//                           strres += QString::number(alreadyssigned, 10);
//                       else
//                           strres += proxyoneline[i];
//                       if (i !=  cnt - 1)
//                           strres += "|";
//                   }

//                   vecStr[idx] = strres;
//                   break;
//                }
//            }
//        }
//    }

    }
FUNC_END:
    qDebug() << "find=" << find;
    if (find == false)
    {
        qDebug() << "func false";
        return -1;
    }
    m_func->writeProxyConf(vecStr);
    qDebug() << "func end";
    return 0;
}

