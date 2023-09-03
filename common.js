function getfuncproxyinfo()
{
    procstatusheader.visible = false;
    functioninfo.visible = false;
    labelhint.visible = true;
    procstatusModel.clear()
    functionModel.clear()
    funcobj.cleardata();
    var res = func.readProcConf()
//    console.log("res=11"+res);
    var resdata = JSON.parse(res);
    var str = "";
    var idx = 0;
    ids.length = 0;
     var proxymode = "";
     var stratery = "";
     var subproc = "";
     var virtual = "";
     var dns = "";
     var limit = "";
     var filter = "";
    procids.length=0
//    procids.clear()
     while (1)
     {
        str = "proc" + String(idx);
        idx++;
        if (typeof(resdata[str]) == "undefined")
        {
             break;
        }

        console.log("id="+resdata[str].id)
        procids.push(resdata[str].id);

        if (resdata[str].proxymode === "0")
        {
            proxymode = "TCP&UDP";
        }

        if (resdata[str].stratery === "0")
        {
            stratery = "顺序";
        }
        else if (resdata[str].stratery === "1")
        {
            stratery = "倒序";
        }
        else if (resdata[str].stratery === "2")
        {
            stratery = "均衡";
        }
        else if (resdata[str].stratery === "3")
        {
            stratery = "随机";
        }
        else if (resdata[str].stratery === "4")
        {
            stratery = "绑模拟器";
        }
        else if (resdata[str].stratery === "5")
        {
            stratery = "绑定窗口";
        }
        else if (resdata[str].stratery === "6")
        {
            stratery = "依命令行";
        }


        if (resdata[str].subproc === "true")
        {
            subproc = "代理";
        }
        else if (resdata[str].subproc === "false")
        {
            subproc = "不代理";
        }


        if (resdata[str].virtual === "false")
        {
            virtual = "假";
        }
        else if (resdata[str].virtual === "true")
        {
            virtual = "真";
        }

        if (resdata[str].limit === "false")
        {
            limit = "假";
        }
        else if (resdata[str].limit === "true")
        {
            virtual = "真";
        }


//        console.log("filter="+resdata[str].filterstatus)
        if (resdata[str].filterstatus === "0")
        {
            filter = "未启用";
        }
        else if (resdata[str].filterstatus === "1")
        {
            filter = "不过滤";
        }
        else if (resdata[str].filterstatus === "2")
        {
            filter = "仅过滤";
        }

        funcobj.addData(resdata[str].proc, resdata[str].MD5, resdata[str].path, proxymode,
                    stratery, subproc, virtual, limit,
                    filter)
     }
}



function selectfunction()
{
    var i;
    for (i=0; i<ids.length; i++)
    {
//        console.log("id=="+ids[i]);
    }

    var resdata = selecgids.split(",");
    var len = resdata.length;
    if (len > 0)
    {
        len = len - 1;
        var tmpids = []

        for (i=0; i<len; i++)
        {
            console.log("selecgids="+selecgids+" id="+resdata[i]);
            tmpids.push(resdata[i])
        }

        functioninfo.setRowRange1(tmpids);
    }
}


function getproxyinfo11()
{
    console.log("getproxyinfo11: func begin");
    functionModel.clear()
    var desc = operProxy.readProxyConf()
    var objinfo = JSON.parse(desc);

    var str = "";
    var idx = 0;

    ids.length = 0;
    while (1)
    {
       str = "proxy" + String(idx);
       idx++;
       if (typeof(objinfo[str]) == "undefined")
       {
            break;
       }

       ids.push(objinfo[str].id)
       var status = "启用";
       var dir = "file:///"+appDir+"/succeed.png";
       if ('0' === objinfo[str].status)
       {
           status = "禁用"
           dir = "file:///"+appDir+"/ban.png";
       }

           functionModel.append({"id":dir,"代理ip":objinfo[str].ip,"端口":objinfo[str].port,
                                                            "类型":objinfo[str].type,"代理账号":objinfo[str].account, "代理密码":objinfo[str].passwd, "代理位置":objinfo[str].pos,
                                                            "线路":objinfo[str].line, "延迟":objinfo[str].delay, "可分配":objinfo[str].canassigned, "已分配":objinfo[str].alreadyassigned
//                                                                      , "到期时间":objinfo[str].deadline, "允许使用进程":objinfo[str].process, "状态":objinfo[str].status})
                                    , "到期时间":objinfo[str].deadline, "实际ip":objinfo[str].ip,"允许使用进程":objinfo[str].process, "状态":status,
                                   "备注":objinfo[str].comment})


    }
}



function setconfinfo()
{
    var res = "";
//    console.log("modifier="+backend.modifier);
//    console.log("processname.checked="+processname.checked);
//    console.log("path.checked="+path.checked);
//    console.log("processMD5.checked="+processMD5.checked);
    if (processname.checked === true)
    {
        res = res + "1|0|0|";
    }
//    else
//    {
//        res = res + "0|";
//    }

    if (path.checked === true)
    {
        res = res + "0|1|0|";
    }
//    else
//    {
//        res = res + "0|";
//    }

    if (processMD5.checked === true)
    {
        res = res + "0|0|1|";
//        res = res + "1|";
    }
//    else
//    {
//        res = res + "0|";
//    }

    if (banlocalnetwork.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (subsameaddr.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (procbeginquit.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (packetvm.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (usesamehardwareaddr.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (pauseproc.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    res = res + textflowgreaterthan.text+"|";

    if (procnoflowtime.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (autoopen.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (min.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (trap.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

   res = res + textsettitle.text+"|";

    if (localanalysis.checked === true)
    {
        res = res + "0|";
    }
    else if (proxyanalysis.checked === true)
    {
        res = res + "1|";
    }
    else if (dnsanalysis.checked === true)
    {
        res = res + "2|";
    }



    res = res + textdnsip.text+"|";
    res = res + textport.text+"|";

    if (openproxy.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (firewallprohibits.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (vmmac.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (vmdiskserial.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (vmdvolumelabel.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (vmdRegistryFeatures.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (vmProtectionIsolation.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (proxychildproc.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    if (virtualhardinfo.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }


     func.setConfInfo(res);
}



function sethighlevelsetting()
{
    var res = ""
    if (setprocpri.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    res = res + textdetquerythd.text+"|";
    res = res + texttimeout.text+"|";
    res = res + textlatencydettimeout.text+"|";

    res = res + textbindwindowtimeout.text+"|";
    if (chkproxyavail.checked === true)
    {
        res = res + "1|";
    }
    else
    {
        res = res + "0|";
    }

    res = res + textdeccycle.text+"|";

    if (driverkernelmode.checked === true)
    {
        res = res + "1|0|";
    }

    if (secialmode.checked === true)
    {
        res = res + "0|1|";
    }

    if (driverprocessmode.checked === true)
    {
        res = res + "1|0|";
    }

    if (commonprocessmode.checked === true)
    {
        res = res + "0|1|";
    }

    func.sethighlevelsetting(res);
}
