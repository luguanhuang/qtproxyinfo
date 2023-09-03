import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12 as Ctrl14

import QtQuick.Controls.Styles 1.4

import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import "common.js" as Common
Rectangle {
    id: topLevel
    property int clickrowinfo: -1
    width: 1834    //+40
    height: 1020       //+40
    visible: true

    property int offset: 0
    property var indexes: []
    property var ids: []
    property var procids: []
    property int __clicks:0
    SystemPalette{ id: syspal }
    property int __previousRow: -1
    property int __modifiers: 0
    property int __firstKeyRow: -1
    property int __isfullsel:0
    property   var obj

   ApplicationWindow
   {
      id: windowObj

//      color: "blue"
      height:530 ; width: 1334


          property string selecgids: ""
          property int clickrowinfo: -1

      //    property int procclickrowinfo: -1

      StackView
     {
         initialItem: editproxy
         id: stackView
        focus: true
        anchors.fill: parent
        Component.onCompleted: console.log("complete")
        onCurrentItemChanged: {
              currentItem.forceActiveFocus()
          }
      }

      Rectangle
      {
          width: 1334    //+40
          height: 520       //+40

 //         width:640
 //         height:480
          color:"#cfcfc0"
          DropArea
          {
              id: droparea
              anchors.fill: parent

              property bool hasUrls: false
              enabled: true

              onDropped:
              {
                  if(drop.hasUrls)
                  {
                      var files = drop.urls
                      console.log("drop files:",files)
                      console.log(drop.urls[0].slice(8,)); //去掉前缀：file:///
                      var result = func.addDragInfo(drop.urls[0].slice(8,));
                      if (0 === result)
                      {
                         Common.getfuncproxyinfo()
                      }
                  }
              }
              onEntered:
              {
                  console.log("extered.")
              }
          }

      }

         Loader
         {
             id:loader
         }

         Editproxy
         {
             id: editproxy
             visible:false
             Component.onCompleted:
             {
                 console.log("complete")
             }
          }

         Component{
                           id:editproxy1
                           Editproxy{
        //                       anchors.centerIn: parent
                                Component.onDestruction:
                               {

                              console.log("Destruction Beginning!")
                             }
                           }
                       }



            function getproxyinfo()
            {
                Common.getproxyinfo11();
            }


             Connections
             {
                     target: operProxy   //指定target
                     onMyClassInfoSignal:
                     {
                         console.log("haha C++ signal is coming11111111111111...")
                         testfunction();
                         }
               }

             Connections
             {
                     target: func   //指定target
                     onUpdateproxyinfo:
                     {
                         console.log("update proxy")
//                         testfunction();
                     }
               }

             Label
             {
                 id: labelhint
                 x: 0; y: 42
                 width: 80
                 height: 20
                 focus: true
                 text: qsTr("右键添加或将要代理的进程拖进下面:") ;
        //         font.pointSize: 10
                 visible: false
             }


//             Image{
//                        id:img
//                        x: 1060; y: 32
//                        width:30
//                        height: 30
//            //            anchors.centerIn: parent
//                        source: "file:///"+appDir+"/succeed.png";
//                    }

                 MouseArea { //为窗口添加鼠标事件
                         id: mouseArea
                         anchors.fill: parent
                         acceptedButtons: Qt.LeftButton //只处理鼠标左键
                         onPressed: { //接收鼠标按下事件
                                console.log("onPressed")
            //                 xMouse = mouseX
            //                 yMouse = mouseY
            //                 mainWindowX = mainWindow.x
            //                 mainWindowY = mainWindow.y
                         }

                         onReleased: { //接收鼠标按下事件
                             console.log("onReleased")
                             console.log("x :",mouseArea.mouseX," y = ",mouseArea.mouseY)
                             var mousePos = mapToGlobal(mouseX, mouseY)
//                             console.log("Mouse position in main window: (" + mousePos.x + ", " + mousePos.y + ")")


            //                 xMouse = mouseX
            //                 yMouse = mouseY
            //                 mainWindowX = mainWindow.x
            //                 mainWindowY = mainWindow.y
                         }



                         onPositionChanged: { //鼠标按下后改变位置
//                             console.log("onPositionChanged")
            //                 mainWindow.x = mainWindowX + (mouseX - xMouse)
            //                 mainWindow.y = mainWindowX + (mouseY - yMouse)
                         }
                     }

                 Label
                 {
                     id: labelhint12
                     x: 0; y: 42+230
                     width: 80
                     height: 20
                     focus: true
                     text: qsTr("识别方式") ;
            //         font.pointSize: 10
                     visible: true
                 }



                 Component
                {

                       id: radioStyle;
                       RadioButtonStyle{

                           indicator: Rectangle{
                               implicitHeight: 12;
                               implicitWidth: 16;

                               radius: 6;
                               border.color: control.hovered? "darkblue":"gray";
                               border.width: 1;

                               Rectangle{
                                   anchors.fill: parent;
                                   visible: control.checked;
                                   color:"#0000a0";
                                   radius: 5;
                                   anchors.margins: 3;
                               }


                           }



                           label:Text{
                               color: control.activeFocus?"blue":"black";
                               text:control.text;
                           }


                       }

                   }

                 //定义的互斥分组
                       ExclusiveGroup{

                           id: mos;
                       }

                          RadioButton
                          {

                                id: processname
                                anchors.top: labelhint12.top; anchors.topMargin: 15
                                anchors.left: labelhint12.left; anchors.leftMargin: 0

                                focus: true
                                text: qsTr("进程名识别") ;
                                checked: true
                                exclusiveGroup: mos;
                              activeFocusOnPress:  true;
                              style:radioStyle;
                             onClicked:
                             {
                                 console.log("name")
                                  Common.setconfinfo()
                             }
                          }


                          RadioButton
                          {
                                id: path
                                anchors.top: labelhint12.top; anchors.topMargin: 15
                                anchors.left: labelhint12.left; anchors.leftMargin: 100

                                focus: true
                                text: qsTr("进程路径识别") ;
                                checked: false
                                exclusiveGroup: mos;
                              activeFocusOnPress:  true;
                              style:radioStyle;
                              onClicked:
                              {
                                  console.log("name")
                                   Common.setconfinfo()
                              }
                           }

        RadioButton
      {
            id: processMD5
            anchors.top: labelhint12.top; anchors.topMargin: 15
            anchors.left: labelhint12.left; anchors.leftMargin: 200

            focus: true
            text: qsTr("进程MD5识别") ;
            checked: false
            exclusiveGroup: mos;
          activeFocusOnPress:  true;
          style:radioStyle;
          onClicked:
          {
              console.log("name")
               Common.setconfinfo()
          }
       }

        Label
                {
                    id: setting
                    anchors.top: processname.top; anchors.topMargin: 25
                    anchors.left: processname.left; anchors.leftMargin: 0
                    width: 80
                    height: 20

       //             x: 0; y: 42+230
                    focus: true
                    text: qsTr("功能设置") ;
           //         font.pointSize: 10
                    visible: true
                }

                CheckBox
                {
                      id: banlocalnetwork
                      anchors.top: setting.top; anchors.topMargin: 15
                      anchors.left: setting.left; anchors.leftMargin: 0

                      width: 90
                      height: 20
                      focus: true
                      text: qsTr("禁止本地网络") ;
                      checked: false
                      onClicked:
                      {
                           console.log("banlocalnetwork")

                          Common.setconfinfo()
                      }
                 }


                CheckBox
                {
                    id: subsameaddr
                    anchors.top: setting.top; anchors.topMargin: 15
                    anchors.left: setting.left; anchors.leftMargin: 100

                    width: 150
                    height: 20
                    focus: true
                    text: qsTr("子进程使用相同过滤地址") ;
                    checked: false
                    onClicked:
                    {
                         console.log("subsameaddr")
                        Common.setconfinfo()
                    }

                }

                CheckBox
                {
                    id: procbeginquit
                    anchors.top: setting.top; anchors.topMargin: 15
                    anchors.left: setting.left; anchors.leftMargin: 260

                    width: 190
                    height: 20
                    focus: true
                    text: qsTr("使用高并发捕获进程开启和退出") ;
                    checked: false
                    onClicked:
                    {
                         console.log("subsameaddr")
                        Common.setconfinfo()
                    }

                }

                QtObject {
                                    id: backend
                                    property int modifier
                                }

                ComboBox
                {
                    id: drivertype
                    anchors.top: setting.top; anchors.topMargin: 15
                    anchors.left: setting.left; anchors.leftMargin: 460

                    width: 120
                    height: 24;
                    textRole: "text"
                    onActivated:
                    {
                        console.log("currentText333="+currentText);
                        if ("普通模式" == currentText)
                        {
                            backend.modifier = 0;
                        }
                        else
                        {
                            backend.modifier = 1;
                        }

                        console.log("subsameaddr")
                       Common.setconfinfo()
                    }

                    Component.onCompleted:
                    {

                        currentIndex = indexOfValue(backend.modifier)
                    }

                    model: [{ value: 0, text: qsTr("普通模式") },
                    { value: 1, text: qsTr("特殊模式") }
            //        { value: Qt.ControlModifier, text: qsTr("Control") }
                    ]
            //        model:ListModel{    //定义这个接口来动态添加下拉内容
            //        id:lmd
            //        }
                    // model: ["First", "Second", "Third"]
               }

                CheckBox
                {

                          id: packetvm
                          anchors.top: banlocalnetwork.top; anchors.topMargin: 15
                          anchors.left: banlocalnetwork.left; anchors.leftMargin: 0

                          width: 90
                          height: 20
                          focus: true
                          text: qsTr("特征封包虚拟") ;
                          checked: false
                          onClicked:
                          {
                               console.log("subsameaddr")
                              Common.setconfinfo()
                          }
                 }


                CheckBox
                {
                    id: usesamehardwareaddr
                    anchors.top: banlocalnetwork.top; anchors.topMargin: 15
                    anchors.left: banlocalnetwork.left; anchors.leftMargin: 100

                    width: 150
                    height: 20
                    focus: true
                    text: qsTr("子进程使用相同硬件地址") ;
                    checked: false
                    onClicked:
                    {
                         console.log("subsameaddr")
                        Common.setconfinfo()
                    }

                }

                CheckBox
                {
                    id: pauseproc
                    anchors.top: banlocalnetwork.top; anchors.topMargin: 15
                    anchors.left: banlocalnetwork.left; anchors.leftMargin: 260

                    width: 145
                    height: 20
                    focus: true
                    text: qsTr("条件匹配前暂停此进程") ;
                    checked: false
                    onClicked:
                    {
                         console.log("subsameaddr")
                        Common.setconfinfo()
                    }

                }


                CheckBox
                {

                      id: procnoflowtime
                      anchors.top: packetvm.top; anchors.topMargin: 15
                      anchors.left: packetvm.left; anchors.leftMargin: 0

                      width: 100
                      height: 20
                      focus: true
                      text: qsTr("进程无流量时间>") ;
                      checked: false
                      onClicked:
                      {
                           console.log("subsameaddr")
                          Common.setconfinfo()
                      }
                  }

                //确认按钮
                Rectangle
                {
                    id: flowgreaterthan
                    width: 40; height: 18;
                    radius: 4
            //        color: "lightgrey"
                    border.color: "grey"

                    anchors.top: packetvm.top; anchors.topMargin: 18
                    anchors.left: packetvm.left; anchors.leftMargin: 110

       //             anchors.top: rectproxyip.top; anchors.topMargin: 34
       //             anchors.left: rectproxyip.left; anchors.leftMargin: 0
                    TextInput
                    {
                        id: textflowgreaterthan
                        anchors.fill: parent
                        anchors.margins: 2
                        font.pointSize: 10
                        focus: true
                   }
                }

                Label
                {
                    id: settingstatic
                    anchors.top: packetvm.top; anchors.topMargin: 21
                    anchors.left: packetvm.left; anchors.leftMargin: 150
                    width: 80
                    height: 20

       //             x: 0; y: 42+230
                    focus: true
                    text: qsTr("秒关闭或取消该进程(建议不低于30)") ;
           //         font.pointSize: 10
                    visible: true
                }

                CheckBox
                {
                      id: autoopen
                      anchors.top: procnoflowtime.top; anchors.topMargin: 22
                      anchors.left: procnoflowtime.left; anchors.leftMargin: 0

                      width: 90
                      height: 20
                      focus: true
                      text: qsTr("开机自动启动") ;
                      checked: false
                      onClicked:
                      {
                           console.log("subsameaddr")
                          Common.setconfinfo()
                      }
                  }

    CheckBox
    {
          id: min
          anchors.top: procnoflowtime.top; anchors.topMargin: 22
          anchors.left: procnoflowtime.left; anchors.leftMargin: 100

          width: 90
          height: 20
          focus: true
          text: qsTr("启动后最小化") ;
          checked: false
          onClicked:
          {
               console.log("subsameaddr")
              Common.setconfinfo()
          }
      }

    CheckBox
            {
                  id: trap
                  anchors.top: procnoflowtime.top; anchors.topMargin: 22
                  anchors.left: procnoflowtime.left; anchors.leftMargin: 200

                  width: 90
                  height: 20
                  focus: true
                  text: qsTr("最小化倒托盘") ;
                  checked: false
                  onClicked:
                  {
                       console.log("subsameaddr")
                      Common.setconfinfo()
                  }
              }

            Label
            {
                id: settitle
                anchors.top: procnoflowtime.top; anchors.topMargin: 26
                anchors.left: procnoflowtime.left; anchors.leftMargin: 320
                width: 80
                height: 20

   //             x: 0; y: 42+230
                focus: true
                text: qsTr("设置本软件标题:") ;
       //         font.pointSize: 10
                visible: true
            }

            CheckBox
            {
                  id: vmmac
                  anchors.top: procnoflowtime.top; anchors.topMargin: 40
                  anchors.left: procnoflowtime.left; anchors.leftMargin: 0

                  width: 90
                  height: 20
                  focus: true
                  text: qsTr("虚拟网卡MAC") ;
                  checked: false
                  onClicked:
                  {
                       console.log("subsameaddr")
                      Common.setconfinfo()
                  }
              }

            CheckBox
            {
                  id: vmdiskserial
                  anchors.top: procnoflowtime.top; anchors.topMargin: 40
                  anchors.left: procnoflowtime.left; anchors.leftMargin: 100

                  width: 90
                  height: 20
                  focus: true
                  text: qsTr("虚拟硬盘序列号") ;
                  checked: false
                  onClicked:
                  {
                       console.log("subsameaddr")
                      Common.setconfinfo()
                  }
              }

            CheckBox
            {
                  id: vmdvolumelabel
                  anchors.top: procnoflowtime.top; anchors.topMargin: 40
                  anchors.left: procnoflowtime.left; anchors.leftMargin: 200

                  width: 90
                  height: 20
                  focus: true
                  text: qsTr("虚拟分区卷标号") ;
                  checked: false
                  onClicked:
                  {
                       console.log("subsameaddr")
                      Common.setconfinfo()
                  }
              }

            CheckBox
            {
                  id: vmdRegistryFeatures
                  anchors.top: procnoflowtime.top; anchors.topMargin: 40
                  anchors.left: procnoflowtime.left; anchors.leftMargin: 300

                  width: 90
                  height: 20
                  focus: true
                  text: qsTr("虚拟注册表特征") ;
                  checked: false
                  onClicked:
                  {
                       console.log("subsameaddr")
                      Common.setconfinfo()
                  }
              }

            CheckBox
            {
                  id: vmProtectionIsolation
                  anchors.top: procnoflowtime.top; anchors.topMargin: 40
                  anchors.left: procnoflowtime.left; anchors.leftMargin: 400

                  width: 90
                  height: 20
                  focus: true
                  text: qsTr("进程强力保护隔离") ;
                  checked: false
                  onClicked:
                  {
                       console.log("subsameaddr")
                      Common.setconfinfo()
                  }
              }

            //确认按钮
            Rectangle
            {
                id: rectsettitle
                width: 200; height: 18;
                radius: 4
        //        color: "lightgrey"
                border.color: "grey"

                anchors.top: procnoflowtime.top; anchors.topMargin: 26
                anchors.left: procnoflowtime.left; anchors.leftMargin: 420

   //             anchors.top: rectproxyip.top; anchors.topMargin: 34
   //             anchors.left: rectproxyip.left; anchors.leftMargin: 0
                TextInput
                {
                    id: textsettitle
                    anchors.fill: parent
                    anchors.margins: 2
                    font.pointSize: 10
                    focus: true
               }
            }

            Ctrl14.TabBar
            {
               id: bar
               width: 500
               height: 27
               visible: true
               x:0
               y:10

                Ctrl14.TabButton
                {
                   id: control
                    contentItem: Text
                    {
                           text: control.text
                           font: control.font
                           opacity: enabled ? 1.0 : 0.3
                           color: bar.currentIndex == 0 ? "red" : "black"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                    }


                    background: Rectangle
                    {
       //                    color: "white"
       //                     color: bar.currentIndex == 0 ? "orange" : "green"
                    }

                    text: qsTr("代理状态")
                    onClicked:
                    {
                        libraryModel.clear()
                        libraryModel1.clear()
                        console.log("test0");
                        header.visible = true;
                        header1.visible = false;
                       labelhint.visible = false;
                        for (var i=0; i<20; i++)
                        {
   //                         libraryModel.append({"进程名":"112","PID":"332","代理IP":"33.33.33.33",
   //                                                                           "MAC":"11223","连接":"112", "发送":"223", "接收":"221",
   //                                                                           "地理位置":"香港", "代理时长":"112", "comm":"112"})
                        }
                    }
                }

                Ctrl14.TabButton
                {

                    id: control1
                     contentItem: Text {
                            text: control1.text
                            font: control1.font
                            opacity: enabled ? 1.0 : 0.3
        //                    color: control.down ? "#17a81a" : "#21be2b"
                            color: bar.currentIndex == 1 ? "red" : "black"
       //                     color: "black"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                    background: Rectangle {
                           color: "white"
                       }
                    text: qsTr("功能")
                    onClicked:
                    {
                        console.log("modifier=" + backend.modifier);
                        labelhint.visible = true;
                        var component = Qt.createComponent("function.qml");
                        if(component.status === Component.Ready)
                        {
                           obj =   component.createObject(parent,{"x": -330, "y": 54});
                           Common.getfuncproxyinfo()
                            var data = func.getConfInfo()

                            var idx = 0;
                            var str = "proc" + String(idx);
                            var resdata = JSON.parse(data);
                            if (typeof(resdata[str]) == "undefined")
                               return;

                            if (resdata[str].processname === "1")
                               processname.checked = true;

                            if (resdata[str].path === "1")
                               path.checked = true;

                            if (resdata[str].processMD5 === "1")
                               processMD5.checked = true;

                            if (resdata[str].isbannetwork === "1")
                               banlocalnetwork.checked = true;

                            if (resdata[str].issubsameaddr === "1")
                               subsameaddr.checked = true;

                            if (resdata[str].iscapture === "1")
                               procbeginquit.checked = true;

                            if (resdata[str].packetvitual === "1")
                               packetvm.checked = true;

                            if (resdata[str].issubsamehardaddr === "1")
                               usesamehardwareaddr.checked = true;

                            if (resdata[str].issuspendprocess === "1")
                               pauseproc.checked = true;


                            if (resdata[str].isstart === "1")
                               autoopen.checked = true;

                            if (resdata[str].ismin === "1")
                               min.checked = true;

                            if (resdata[str].istrap === "1")
                               trap.checked = true;



   //                         if (resdata[str].title === "1")
                            var pos = resdata[str].processnoflowtime.indexOf(":");
                             var first = resdata[str].processnoflowtime.substr(0, pos);
                            var sec = resdata[str].processnoflowtime.substr(pos+1);
                            textsettitle.text = resdata[str].title;
                            if (first === "1")
                               procnoflowtime.checked = true;
                           textflowgreaterthan.text = sec;

   //                        console.log("data="+resdata[str].processnoflowtime + " first=" + first + " sec="+sec)
                        }
                     }

                }

                Ctrl14.TabButton
                {
                   id: control2
                   contentItem: Text
                   {
                        text: control2.text
                        font: control2.font
                        opacity: enabled ? 1.0 : 0.3
                        color: bar.currentIndex == 2 ? "red" : "black"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                   }
                    background: Rectangle
                    {
       //                    color: "#f4d37c"
                       }
                    text: qsTr("节点管理")
                    onClicked:
                    {
                        labelhint.visible = false;
                        libraryModel.clear()
                        libraryModel1.clear()
                        console.log("test2");
                        header.visible = false;
                        header1.visible = true;
                        var desc = operProxy.readProxyConf()

                        var objinfo = JSON.parse(desc);

                        var str = "";
                        ids.length = 0;
                        var idx = 0;
                        while (1)
                        {
                           str = "proxy" + String(idx);
                           idx++;
                           if (typeof(objinfo[str]) == "undefined")
                           {
   //                            console.log("undefined str="+str)
                                break;
                           }


                           ids.push(objinfo[str].id)
   //                        console.log("id="+objinfo[str].id)
   //                        if ("1" === objinfo[str].status)

   //                        console.log("id1111111="+objinfo[str].id+" ip="+objinfo[str].ip
   //                                    +" ip="+objinfo[str].ip
   //                                    +" port="+objinfo[str].port
   //                                    +" account="+objinfo[str].account
   //                                    +" passwd="+objinfo[str].passwd
   //                                    +" deadline="+objinfo[str].deadline
   //                                    +" type="+objinfo[str].type
   //                                    +" pos="+objinfo[str].pos
   //                                    +" line="+objinfo[str].line
   //                                    +" delay="+objinfo[str].delay
   //                                    +" comment="+objinfo[str].comment
   //                                    +" alreadyassigned="+objinfo[str].alreadyassigned
   //                                    +" process="+objinfo[str].process
   //                                    +" status="+objinfo[str].status
   //                                    +" canassigned="+objinfo[str].canassigned);
                           var status = "启用";
                            var dir = "file:///"+appDir+"/succeed.png";
                            if ('0' === objinfo[str].status)
                            {

                                status = "禁用"
                                dir = "file:///"+appDir+"/ban.png";
                            }



                            libraryModel1.append({"id":dir,"num":String(1), "代理ip":objinfo[str].ip,"端口":objinfo[str].port,
                                                                                "类型":objinfo[str].type,"代理账号":objinfo[str].account, "代理密码":objinfo[str].passwd, "代理位置":objinfo[str].pos,
                                                                                "线路":objinfo[str].line, "延迟":objinfo[str].delay, "可分配":objinfo[str].canassigned, "已分配":objinfo[str].alreadyassigned
                                                        , "到期时间":objinfo[str].deadline, "实际ip":objinfo[str].ip,"允许使用进程":objinfo[str].process, "状态":status,
                                                       "备注":objinfo[str].comment})
   //                         idx = idx+1


                        }
                    }

                }
             }



            Component{
                            id:editproxy11
                            Editproxy{
                                  id:editproxy22
   //                             anchors.centerIn: parent
                                 Component.onDestruction:
                                {

                               console.log("Destruction Beginning!")
                              }
                            }
                        }

            MessageDialog
            {
                id:resprocDialog
                title: "返回框"
                text: "修改设备成功"
                icon: StandardIcon.Information;
                standardButtons:StandardButton.Ok;
                property int iflush: 1
                Component.onCompleted: visible = false

                onButtonClicked:
                {
                   Common.getfuncproxyinfo()
                 }
            }

            MessageDialog
            {
                id:resDialog
                title: "返回框"
                text: "修改设备成功"
                icon: StandardIcon.Information;
                standardButtons:StandardButton.Ok;
                property int iflush: 1
                Component.onCompleted: visible = false

                onButtonClicked:
                {
                    if (1== iflush)
                     {
                        libraryModel.clear()
                        libraryModel1.clear()
                        console.log("test2");
                         header.visible = false;
                         header1.visible = true;
                         var desc = operProxy.readProxyConf()
                         var objinfo = JSON.parse(desc);
                        var str = "";
                         var idx = 0;
       //                     ids.clear();
                       ids.length = 0;
                         while (1)
                         {
                            str = "proxy" + String(idx);
                            idx++;
                            if (typeof(objinfo[str]) == "undefined")
                            {
       //                            console.log("undefined")
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
       //                     if ("1" === objinfo[str].status)
                              libraryModel1.append({"id":dir,"代理ip":objinfo[str].ip,"端口":objinfo[str].port,
                                                                                 "类型":objinfo[str].type,"代理账号":objinfo[str].account, "代理密码":objinfo[str].passwd, "代理位置":objinfo[str].pos,
                                                                                 "线路":objinfo[str].line, "延迟":objinfo[str].delay, "可分配":objinfo[str].canassigned, "已分配":objinfo[str].alreadyassigned
        //                                                                      , "到期时间":objinfo[str].deadline, "允许使用进程":objinfo[str].process, "状态":objinfo[str].status})
                                                         , "到期时间":objinfo[str].deadline, "允许使用进程":objinfo[str].process, "状态":status,
                                                        "备注":objinfo[str].comment})


                         }
                    }


   //
                   }
            }
           Loader { id: pageLoader   }

           FileDialog
           {
               id: idFileOpenOne
               title: "Please choose a file"
                   folder: shortcuts.home
   //            selectFolder: true
   //            selectExisting: true
   //            selectMultiple: true
   //            fileMode: FileDialog.OpenFile
   //            nameFilters: ["Pictures (*.png *.jpg *.gif *.bmp)", "All (*.*)"]
   //            onAccepted: {
   //                    console.log("You chose: " + fileDialog.fileUrls)
   //                    Qt.quit()
   //                }

   //            onRejected: {
   //                    console.log("Canceled")
   //                    Qt.quit()
   //                }
   //                Component.onCompleted: visible = true
   //            options :FileDialog.ReadOnly
           }

           TableView
                  {
                      id:functionView
                      width: parent.width
                      height: parent.height
                      anchors.top: parent.top; anchors.topMargin: 250+offset
                     anchors.left: parent.left
                     anchors.right: parent.right
                     anchors.bottom: parent.bottom
                     clip: true
                     visible: false

                      TableViewColumn
                      {

                          role: "端口"
                          title: "端口"
                          width: 50
                      }
                      TableViewColumn
                      {
                          role: "类型"
                          title: "类型"
                          width: 60

                      }

                      TableViewColumn
                      {

                          role: "代理账号"
                          title: "代理账号"
                          width: 80
                      }
                      TableViewColumn
                      {
                          role: "代理密码"
                          title: "代理密码"
                          width: 80

                      }

                      TableViewColumn
                      {
                          role: "可分配"
                          title: "可分配"
                          width: 70
                      }

                      TableViewColumn
                      {
                          role: "已分配"
                          title: "已分配"
                          width: 60
                      }
                      TableViewColumn
                      {
                          role: "到期时间"
                          title: "到期时间"
                          width: 130
                      }

                      TableViewColumn
                      {
                          role: "实际ip"
                          title: "实际ip"
                          width: 100

                      }

                      TableViewColumn
                      {
                          role: "允许使用进程"
                          title: "允许使用进程"
                          width: 150
                      }

                      TableViewColumn
                      {

                          role: "代理位置"
                          title: "代理位置"
                          width: 80
                      }
                      TableViewColumn
                      {
                          role: "线路"
                          title: "线路"
                          width: 100

                      }

                      TableViewColumn
                      {

                          role: "延迟"
                          title: "延迟"
                          width: 40
                      }

                      TableViewColumn
                      {
                          role: "状态"
                          title: "状态"
                          width: 45
                      }

                      TableViewColumn
                      {
                          role: "备注"
                          title: "备注"
                          width: 65
                      }

                      model: ListModel
                     {

                         id: functionView1
                     }
                  }


                   MessageDialog
                   {
                       id:resDialog1
                       title: "返回框"
                       text: "修改设备成功"
                       icon: StandardIcon.Information;
                       standardButtons:StandardButton.Ok;

                       Component.onCompleted: visible = false

                       onButtonClicked:
                       {
          //                 bataddproxy.hide()
                           console.log("before close");
          //                 bataddproxy.close()
                           console.log("after close");
                           libraryModel.clear()
                           libraryModel1.clear()
                           console.log("test2");
                            header.visible = false;
                            header1.visible = true;
                            var desc = operProxy.readProxyConf()
                            var objinfo = JSON.parse(desc);
                           var str = "";
                            var idx = 0;
          //                     /*ids*/.clear();
                          ids.length = 0;
                            while (1)
                            {
                               str = "proxy" + String(idx);
                               idx++;
                               if (typeof(objinfo[str]) == "undefined")
                               {
          //                            console.log("undefined")
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
                                   libraryModel1.append({"id":dir,"代理ip":objinfo[str].ip,"端口":objinfo[str].port,
                                                                                    "类型":objinfo[str].type,"代理账号":objinfo[str].account, "代理密码":objinfo[str].passwd, "代理位置":objinfo[str].pos,
                                                                                    "线路":objinfo[str].line, "延迟":objinfo[str].delay, "可分配":objinfo[str].canassigned, "已分配":objinfo[str].alreadyassigned
           //                                                                      , "到期时间":objinfo[str].deadline, "允许使用进程":objinfo[str].process, "状态":objinfo[str].status})
                                                            , "到期时间":objinfo[str].deadline, "允许使用进程":objinfo[str].process, "状态":status,
                                                           "备注":objinfo[str].comment})


                            }
          //
                          }
                   }

                   Menu {
                           id: proxystatus

                           MenuItem
                           {
                               text: "重新捕获进程"
                               onTriggered:
                                {
                                  var desc = func.reCapture()
                                  var objinfo = JSON.parse(desc);
                                  var str = "";
                                   var idx = 1;

                                   while (1)
                                   {
                                      str = "proc" + String(idx);
                                      idx++;
                                      if (typeof(objinfo[str]) == "undefined")
                                      {
                 //                            console.log("undefined")
                                           break;

                                      }

                                       libraryModel.append({"进程名":objinfo[str].proc,"PID":objinfo[str].pid,"代理IP":objinfo[str].proxyip,
                                                                             "MAC":objinfo[str].mac,"连接":objinfo[str].connection,
                                                                              "发送":objinfo[str].sendnum, "接收":objinfo[str].recvnum,
                                                                             "地理位置":objinfo[str].pos, "代理时长":"12",
                                                                              "comm":"", "comm1":objinfo[str].proc})
                                   }
          //                        console.log("ret="+ret)
          //                             addproxy.show();
          //                            pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                }
                           }
                       }

                   Menu {
                           id: procinfo

                           Menu
                           {
                               title: "添加"
                               MenuItem
                               {
                                   text: "路径或文件"
                                   onTriggered:
                                    {
          //                                idFileOpenOne.open();
          //                             addproxy.show();
                                        var result = func.addPath();
          //                                pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                    }
                               }

                               MenuItem
                               {
                                   text: "进程名"
                                   onTriggered:
                                    {

//                                        var result = func.addPath();
                                      pageLoader.setSource("qrc:/AddProc.qml", {"containerqml": windowObj});
                                    }
                               }

                               Menu
                               {
                                   title: "安卓模拟器"
                                   MenuItem
                                   {
                                       text: "雷电"
                                       onTriggered:
                                        {
                                           var result = func.addProcInfo(0);
                                           if (0 === result)
                                           {
                                              Common.getfuncproxyinfo()
                                           }
                                        }
                                   }

                                   MenuItem
                                   {
                                       text: "逍遥"
                                       onTriggered:
                                        {
              //                             addproxy.show();
                                              pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                        }
                                   }

                                   MenuItem
                                   {
                                       text: "夜神"
                                       onTriggered:
                                        {
              //                             addproxy.show();
                                              pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                        }
                                   }

                                   MenuItem
                                   {
                                       text: "雷神"
                                       onTriggered:
                                        {
              //                             addproxy.show();
          //                                    pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                              func.addProcInfo(0);
                                        }
                                   }

                                   MenuItem
                                   {
                                       text: "蓝叠"
                                       onTriggered:
                                        {
              //                             addproxy.show();
                                              pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                        }
                                   }

                                   MenuItem
                                   {
                                       text: "MuMu"
                                       onTriggered:
                                        {
              //                             addproxy.show();
                                              pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                        }
                                   }

                                   MenuItem
                                   {
                                       text: "寻找添加已开启的模拟器"
                                       onTriggered:
                                        {
              //                             addproxy.show();
                                              pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                        }
                                   }
                               }

                               MenuItem
                               {
                                   text: "默认浏览器"
                                   onTriggered:
                                    {
          //                             addproxy.show();
                                          pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                    }
                               }

                               MenuItem
                               {
                                   text: "VMware"
                                   onTriggered:
                                    {
          //                             addproxy.show();
                                          pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                    }
                               }
                           }
                           MenuItem
                           {
                               text: "编辑"
                               onTriggered:
                                {
                                   console.log("rowinfo="+obj.procclickrowinfo+" id="+procids[obj.procclickrowinfo])
                                  if (obj.procclickrowinfo >= 0)
                                      pageLoader.setSource("qrc:/Editprocess.qml", {"procid": procids[obj.procclickrowinfo]});
                                }
                           }
                           MenuItem
                           {
                               text: "删除"
                               onTriggered:
                                {
                                    console.log("rowinfo="+obj.procclickrowinfo+" id="+procids[obj.procclickrowinfo])
                                   var ret = func.delProcConf(procids[obj.procclickrowinfo]);
                                   if (0 === ret)
                                    {
                                       if (0 === ret)
                                       {
                                           resprocDialog.text = "删除完成,共计选中1,删除1";
                                       }
                                       else
                                       {
          //                                 resprocDialog.text = "删除代理完成,共计选中1,删除成功0, 删除失败1";
                                       }

                                       resprocDialog.open()

                                   }

                                }

                           }
                           MenuItem { text: "打开文件位置" }
                       }

                   Menu {
                          id: nodemanager
                           Menu
                           {
                               title: "添加"
                               MenuItem
                               {
                                   text: "普通添加"
                                   onTriggered:
                                    {
          //                             addproxy.show();
                                          pageLoader.setSource("qrc:/Addproxy.qml", {"containerqml": winmain});
                                    }
                               }

                               MenuItem
                               {
                                   text: "批量添加"
                                   onTriggered:
                                    {
//                                          pageLoader.setSource("qrc:/BatAddproxy.qml", {"containerqml": winmain});
                                       pageLoader.setSource("qrc:/BatAddproxy.qml", {"containerqml": windowObj});
                                    }
                               }
                           }

                           Menu
                           {
                               title: "删除"
                               MenuItem
                               {
                                   text: "删除选中"
                                   onTriggered:
                                    {
                                          console.log(" 代理ip="+libraryModel1.get(clickrowinfo)['代理ip']
                                                      +" id="+ids[clickrowinfo]+" clickrowinfo="+clickrowinfo)
                                          var ret = operProxy.delProxyConf(ids[clickrowinfo])
                                          if (0 === ret)
                                          {
                                              resDialog.text = "删除代理完成,共计选中1,删除成功1, 删除失败0";
                                          }
                                          else
                                          {
                                              resDialog.text = "删除代理完成,共计选中1,删除成功0, 删除失败1";
                                          }

                                          resDialog.open()
          //                               console.log("row11="+nodemanager.clickrowinfo);
          //                             addproxy.show();
                                    }
                               }

                               MenuItem
                               {
                                   text: "删除重复"
                                   onTriggered:
                                    {
                                       console.log("testttt")
                                      var ids = []; //推荐
                                       for (var j=0; j<indexes.length; j++)
                                          {

                                              ids.push(indexes[j]);
                                          }

                                       var s = ids.toString();  //把数组转换为字符串
                                       console.log("s="+s)
                                      operProxy.delRepeatRows(s)
          //                             header1.getCurIds()
          //                             bataddproxy.show();
                                    }
                               }

                               MenuItem
                               {
                                   text: "删除全部"
                                   onTriggered:
                                    {
                                       var res = operProxy.delAll();
                                       resDialog.text = res;
                                      resDialog.iflush = 1;
                                       resDialog.open()
                                    }
                               }

                               MenuItem
                               {
                                   text: "删除无效代码"
                                   onTriggered:
                                    {
                                       bataddproxy.show();
                                    }
                               }
                           }

                           MenuItem
                           {
                               text: "编辑"
                               onTriggered:
                               {
          //                         var dir = "file:///"+appDir+"/succeed.png";
                                   console.log("clickrowinfo="+clickrowinfo)
          //                         console.log("main.qml="+"qrc:/main.qml")
          //                         console.log("id123="+ids[clickrowinfo]);
                                   if (clickrowinfo >= 0)
                                      pageLoader.setSource("qrc:/Editproxy.qml", {"proxyid": ids[clickrowinfo]});
                                }
                           }

                           Menu
                           {
                               title: "快捷"

                               MenuItem
                               {
                                   text: "启用代理"
                                   onTriggered:
                                   {
                                       console.log("use proxy id="+ids[clickrowinfo]);
                                       var total = "启用代码完成, 共计选中1,";
                                       var ret = operProxy.enableproxy(ids[clickrowinfo])
                                      total += ret;
                                      resDialog.text = total;

                                       resDialog.open()
                                   }
                               }

                               MenuItem
                               {
                                   text: "禁用代理"
                                   onTriggered:
                                   {
                                       console.log("disable proxy id="+ids[clickrowinfo]);
                                       var total = "禁用代码完成, 共计选中1,";
                                       var ret = operProxy.disableproxy(ids[clickrowinfo])
                                      total += ret;
                                      resDialog.text = total;

                                       resDialog.open()
                                   }
                               }


                               Menu
                               {
                                   title: "复制为"

                                   MenuItem
                                   {
                                       text: "太极文件格式"
                                       onTriggered:
                                       {
              //                            console.log("use proxy")
                                           console.log("use proxy id="+ids[clickrowinfo]);
                                           var total = "将一个所选代理复制为[太极文件格式]";
                                           var ret = operProxy.copyproxy(ids[clickrowinfo])

                                          total += ret;
                                          resDialog.text = total;
                                          resDialog.iflush = 0;
                                           resDialog.open()
                                       }
                                   }
                               }

                               MenuItem
                               {
                                   text: "设置隧道"
                               }

                               MenuItem
                               {
                                   text: "执行绑定引导"
                               }

                               MenuItem
                               {
                                   text: "修改备注"
                                   onTriggered:
                                   {
                                      pageLoader.setSource("qrc:/Modifycomment.qml", {"proxyid": ids[clickrowinfo], "containerqml": winmain});
                                   }
                               }

                               MenuItem
                               {
                                   text: "修改可分配"
                                   onTriggered:
                                   {
                                      pageLoader.setSource("qrc:/Modifycanassigned.qml", {"proxyid": ids[clickrowinfo], "containerqml": winmain});
                                   }

                               }

                               MenuItem
                               {

                                   text: "修改到期时间"
                                   onTriggered:
                                   {
                                      pageLoader.setSource("qrc:/Modifydeadline.qml", {"proxyid": ids[clickrowinfo], "containerqml": winmain});
                                   }

                               }

                               MenuItem
                               {
                                   text: "修改允许代理的进程"
                                   onTriggered:
                                   {
                                      pageLoader.setSource("qrc:/Modifyallowproxy.qml", {"proxyid": ids[clickrowinfo], "containerqml": winmain});
                                   }
                               }
                            }


                           MenuItem
                           {
                               text: "检测"
                               onTriggered:
                                {
                                  console.log("detect");
                                }
                           }


                           Menu
                           {
                               title: "选中"

                               MenuItem
                               {
                                   text: "全选"
                                   onTriggered:
                                   {
                                       var cnt = operProxy.getfilecnt()
                                       __isfullsel = 1
                                      __modifiers = 0;
                                     __modifiers = __modifiers |= Qt.ControlModifier
                                     __modifiers |= Qt.ShiftModifier
                                       header1.keySelect1(true, cnt)
                                   }
                               }

                               MenuItem
                               {
                                   text: "反选"
                                   onTriggered:
                                   {
                                       var cnt = operProxy.getfilecnt()
                                       __isfullsel = 2
                                      __modifiers = 0;
                                     __modifiers = __modifiers |= Qt.ControlModifier
                                     __modifiers |= Qt.ShiftModifier

          //                             header1.keySelect1(__modifiers & Qt.ShiftModifier, 2)
                                       header1.keySelect1(true, cnt)

                                   }
                               }


                               Menu
                               {
                                   title: "类型"

                                   MenuItem
                                   {
                                       text: "SOCKS5"
                                       onTriggered:
                                       {
                                           var strids = operProxy.getseltypes("SOCKS5")
                                           var resdata = strids.split(",");

                                           var len = resdata.length;

                                            if (len > 0)
                                            {
                                                len = len - 1;
                                            }

                                            var hint = "选中"+String(len)+"个[SOCKS5]类型的代理"

                                            resDialog.text = hint;
                                            resDialog.open()

                                            selecgids = strids
                                            Common.selectfunction();//调用page1.qml中的函数，实现了传返回值。
                                       }
                                   }
                               }

                               MenuItem
                               {
                                   text: "到期时间"
                                   onTriggered:
                                   {
                                      pageLoader.setSource("Seldeadline.qml", {"containerqml": winmain});

                                   }
                               }

                               MenuItem
                               {
                                   text: "备注"
                                   onTriggered:
                                   {
                                      pageLoader.setSource("Selcomment.qml", {"proxyid": ids[clickrowinfo], "containerqml": winmain});

                                   }
                               }

                          }

                           MenuItem
                           {
                               text: "刷新"
                               onTriggered:
                               {
                                   libraryModel.clear()
                                   libraryModel1.clear()
                                   console.log("test2");
                                   header.visible = false;
                                   header1.visible = true;
                                   var desc = operProxy.readProxyConf()
          //                         console.log("desc="+desc);
                                   var objinfo = JSON.parse(desc);

                                   var str = "";
                                   var idx = 0;
              //                     ids.clear();
                                   ids.length = 0;
                                   while (1)
                                   {
                                      str = "proxy" + String(idx);
                                      idx++;
                                      if (typeof(objinfo[str]) == "undefined")
                                      {
              //                            console.log("undefined")
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


                                       libraryModel1.append({"id":dir,"代理ip":objinfo[str].ip,"端口":objinfo[str].port,
                                                                                           "类型":objinfo[str].type,"代理账号":objinfo[str].account, "代理密码":objinfo[str].passwd, "代理位置":objinfo[str].pos,
                                                                                           "线路":objinfo[str].line, "延迟":objinfo[str].delay, "可分配":objinfo[str].canassigned, "已分配":objinfo[str].alreadyassigned
                                                                   , "到期时间":objinfo[str].deadline, "实际ip":objinfo[str].ip,"允许使用进程":objinfo[str].process, "状态":status,
                                                                  "备注":objinfo[str].comment})


                                   }

                               }

                           }
                       }


                   TableView
                           {
                               id:header
                               width: parent.width
                               height: parent.height
                               anchors.top: parent.top; anchors.topMargin: 50+offset


                              anchors.left: parent.left
                              anchors.right: parent.right
                              anchors.bottom: parent.bottom
                              clip: true
                              visible: true
                  //         boundsBehavior: Flickable.OvershootBounds

                               TableViewColumn
                               {
                                   role: "进程名"
                                   title: "进程名"
                                   width: 180
                  //                 delegate:imageDelegate
                               }

                               TableViewColumn
                               {
                                   role: "PID"
                                   title: "PID"
                                   width: 110
                  //                delegate:textDelegate
                               }


                               TableViewColumn
                               {
                                   role: "代理IP"
                                   title: "代理IP"
                                   width: 160
                  //                delegate:textDelegate
                               }

                               TableViewColumn
                               {

                                   role: "MAC"
                                   title: "MAC"
                                   width: 160
                               }

                               TableViewColumn
                               {
                                   role: "连接"
                                   title: "连接"
                                   width: 90

                               }

                               TableViewColumn
                               {

                                   role: "发送"
                                   title: "发送"
                                   width: 90
                               }
                               TableViewColumn
                               {
                                   role: "接收"
                                   title: "接收"
                                   width: 90

                               }

                               TableViewColumn
                               {

                                   role: "地理位置"
                                   title: "地理位置"
                                   width: 110
                               }
                               TableViewColumn
                               {
                                   role: "代理时长"
                                   title: "代理时长"
                                   width: 110
                               }
                               TableViewColumn
                               {
                                   role: "comm"
                                   title: "#"
                                   width: 80
                               }

                               model: ListModel
                              {

                                  id: libraryModel
                              }

                              style:TableViewStyle
                              {
                                  rowDelegate: Rectangle
                                  {
                                      property bool selected: __clicks && indexes.indexOf(styleData.row) > -1
                                      width: parent.width ; height: 20
                                      color: selected ? syspal.highlight : syspal.base
                                      border.color: "whitesmoke"
                                  }

                                  itemDelegate: Text
                                  {
                                      property bool selected: __clicks && indexes.indexOf(styleData.row) > -1
                                      anchors.fill: parent
                                      color: selected ? syspal.highlightedText : syspal.text
                                      anchors.leftMargin: 6
                                      verticalAlignment: Text.AlignVCenter
                                      renderType: Text.NativeRendering
                                      elide: styleData.elideMode
                                      text: styleData.value

                                  }


                              }

                              Component
                              {
                                 id: textDelegate
                                 Item
                                 {
                                     id: f_item
                                     height: cell_txt.height
                                     Text
                                     {
                                         id: cell_txt
                                         width: parent.width
                                         verticalAlignment: Text.AlignVCenter
                                         horizontalAlignment: Text.AlignHCenter
                                         //font.bold: true
                                         text: styleData.value
                                         elide: Text.AlignHCenter
                                         color: "white"
                                         renderType: Text.NativeRendering
                                     }
                                 }
                              }

                              Component
                              {
                                 id: imageDelegate1
                                 Item
                                 {
                                     Image
                                     {
                                         anchors.verticalCenter: parent.verticalCenter
                                         anchors.horizontalCenter: parent.horizontalCenter
                                         fillMode: Image.PreserveAspectFit
                                         height:20
                                         cache : true;
                                         asynchronous: true;
                                         source: styleData.value// !== undefined  ? styleData.value : ""
                                     }
                                 }
                              }

                              // Shift-key selection is relative to the index when shift was pressed
                              Keys.onUpPressed:
                              {
                                  console.log("onUpPressed");
                                  keySelect(__modifiers & Qt.ShiftModifier, currentRow)
                              }

                              Keys.onDownPressed:
                              {
                                  console.log("onDownPressed");
                                  keySelect(__modifiers & Qt.ShiftModifier, currentRow)
                              }


                              function keySelect(shiftPressed, row)
                              {
                                  if (shiftPressed)
                                  {
                                      indexes = []
                                      setRowRange(__modifiers & Qt.ShiftModifier, __firstKeyRow, row)
                                  }
                                  else
                                  {
                                      indexes = [row]
                                  }
                              }

                              Keys.onPressed:
                              {
                                  if (event.key === Qt.Key_Control)
                                      __modifiers |= Qt.ControlModifier
                                  if (event.key === Qt.Key_Shift)
                                  {
                                      __modifiers |= Qt.ShiftModifier
                                      __firstKeyRow = currentRow
                                  }
                              }

                              Keys.onReleased: {
                                  if (event.key === Qt.Key_Control)
                                      __modifiers &= !Qt.ControlModifier
                                  if (event.key === Qt.Key_Shift) {
                                      __modifiers &= !Qt.ShiftModifier
                                      __firstKeyRow = -1
                                  }
                              }

                              MouseArea
                              {
                                  anchors.fill: parent
                                  acceptedButtons: /*Qt.LeftButton | */Qt.RightButton
                                  onClicked: {
                                      console.log("Click11")
                                      if (mouse.button == Qt.RightButton)
                                      {
                                           console.log("Click112222")
                  //                        if (__clicks && indexes.length > -1)
                                          {
                                              console.log("Right")
                                               proxystatus.popup()
                                          }


                                      }
                                  }
                              }

                              onClicked:
                              {
                                  var index = currentRow
                                  if (__modifiers & Qt.ShiftModifier) {
                                      setRowRange(true, __previousRow, index)
                                  } else {
                                      var hasIndex = indexes.indexOf(index) !== -1
                                      if (__modifiers & Qt.ControlModifier) {
                                          setRowRange(!hasIndex, index, index)
                                      } else {
                                          indexes = (indexes.length == 1 && hasIndex) ? [] : [index]
                                          __clicks++  // force a re-evaluation of indexes in the delegates
                                      }
                                  }
                                  __previousRow = index
                              }

                              function setRowRange(select, first, last) {
                                  var start = first <= last ? first : last
                                  var stop = first <= last ? last : first
                                  for (var i = start ; i <= stop ; ++ i) {
                                      var indexAt = indexes.indexOf(i)
                                      if (select) {
                                          if (indexAt < 0)
                                              indexes.push(i)
                                      } else {
                                          if (indexAt > -1)
                                              indexes.splice(indexAt, 1);
                                      }
                                  }
                                  __clicks++  // force a re-evaluation of indexes in the delegates
                              }

                      }

                           TableView
                           {
                               id:header1
                               width: parent.width
                               height: parent.height
                               anchors.top: parent.top; anchors.topMargin: 50+offset
                              anchors.left: parent.left
                              anchors.right: parent.right
                              anchors.bottom: parent.bottom
                              clip: true
                              visible: false

                               TableViewColumn
                               {
                                   role: "id"
                                   title: "id"
                                   width: 20
                                   delegate:imageDelegate
                               }

                               TableViewColumn
                               {
                                   role: "num"
                                   title: "num"
                                   width: 25
                  //                 delegate:imageDelegate
                               }

                               TableViewColumn
                               {
                                   role: "代理ip"
                                   title: "代理ip"
                                   width: 100

                               }

                               TableViewColumn
                               {

                                   role: "端口"
                                   title: "端口"
                                   width: 50
                               }
                               TableViewColumn
                               {
                                   role: "类型"
                                   title: "类型"
                                   width: 60

                               }

                               TableViewColumn
                               {

                                   role: "代理账号"
                                   title: "代理账号"
                                   width: 80
                               }
                               TableViewColumn
                               {
                                   role: "代理密码"
                                   title: "代理密码"
                                   width: 80

                               }

                               TableViewColumn
                               {
                                   role: "可分配"
                                   title: "可分配"
                                   width: 70
                               }

                               TableViewColumn
                               {
                                   role: "已分配"
                                   title: "已分配"
                                   width: 60
                               }
                               TableViewColumn
                               {
                                   role: "到期时间"
                                   title: "到期时间"
                                   width: 130
                               }

                               TableViewColumn
                               {
                                   role: "实际ip"
                                   title: "实际ip"
                                   width: 100

                               }

                               TableViewColumn
                               {
                                   role: "允许使用进程"
                                   title: "允许使用进程"
                                   width: 150
                               }

                               TableViewColumn
                               {

                                   role: "代理位置"
                                   title: "代理位置"
                                   width: 80
                               }
                               TableViewColumn
                               {
                                   role: "线路"
                                   title: "线路"
                                   width: 100

                               }

                               TableViewColumn
                               {

                                   role: "延迟"
                                   title: "延迟"
                                   width: 40
                               }

                               TableViewColumn
                               {
                                   role: "状态"
                                   title: "状态"
                                   width: 45
                               }

                               TableViewColumn
                               {
                                   role: "备注"
                                   title: "备注"
                                   width: 65
                               }

                              model: ListModel
                              {

                                  id: libraryModel1
                              }

                              rowDelegate: Rectangle
                              {
                                  property bool selected: __clicks && indexes.indexOf(styleData.row) > -1
                                  width: parent.width ; height: 20
                                  color: selected ? syspal.highlight : syspal.base
                                  border.color: "whitesmoke"
                              }

                              // 设置表头的样式
                              headerDelegate: Rectangle {
                  //                implicitWidth: 10
                                  implicitHeight: 24
                  //                gradient: styleData.pressed ? root.pressG : (styleData.containsMouse ? root.hoverG: root.normalG)
                                  border.width: 1
                  //                border.color: "gray"

                                  Text {
                                      anchors.verticalCenter: parent.verticalCenter
                                      anchors.left: parent.left
                                      anchors.leftMargin: 4
                                      anchors.right: parent.right
                                      anchors.rightMargin: 4
                                      text: styleData.value
                  //                    color: styleData.pressed ? "red" : "blue"
                  //                    font.bold: true
                                  }
                              }

                              itemDelegate: Text
                              {
                                  property bool selected: __clicks && indexes.indexOf(styleData.row) > -1
                                  anchors.fill: parent
                                  color: selected ? syspal.highlightedText : syspal.text
                                  anchors.leftMargin: 6
                                  verticalAlignment: Text.AlignVCenter
                                  renderType: Text.NativeRendering
                                  elide: styleData.elideMode
                                  text: styleData.value

                              }

                              Component
                                {
                                   id: imageDelegate
                                   Item
                                   {
                                       Image
                                       {
                                           anchors.verticalCenter: parent.verticalCenter
                                           anchors.horizontalCenter: parent.horizontalCenter
                                           fillMode: Image.PreserveAspectFit
                                           height:20
                                           cache : true;
                                           asynchronous: true;
                                           source: styleData.value// !== undefined  ? styleData.value : ""
                                       }
                                   }
                                }

                              // Shift-key selection is relative to the index when shift was pressed
                              Keys.onUpPressed: keySelect(__modifiers & Qt.ShiftModifier, currentRow)
                              Keys.onDownPressed: keySelect(__modifiers & Qt.ShiftModifier, currentRow)

                              function keySelect(shiftPressed, row)
                              {

                                  clickrowinfo = clickrow;
                                  console.log("clickrow112="+clickrowinfo);
                                  if (shiftPressed) {
                                      indexes = []
                                      setRowRange(__modifiers & Qt.ShiftModifier, __firstKeyRow, row)
                                  } else {
                                      indexes = [row]
                                  }
                              }

                              function keySelect1(shiftPressed, row)
                              {
                  //                clickrowinfo = clickrow;
                  //                console.log("clickrow112="+clickrowinfo);
                                  if (shiftPressed)
                                  {
                                      indexes = []
                                      __firstKeyRow = 0
                  //                    setRowRange(__modifiers & Qt.ShiftModifier, __firstKeyRow, row)
                                      setRowRange(1, __firstKeyRow, row)
                                  } else {
                                      indexes = [row]
                                  }
                              }

                              Keys.onPressed: {
                                  if (event.key === Qt.Key_Control)
                                  {
                                         console.log("onPressed ControlModifier");
                                         __modifiers |= Qt.ControlModifier
                                  }

                                  if (event.key === Qt.Key_Shift)
                                  {
                                      console.log("onPressed ShiftModifier");
                                      __modifiers |= Qt.ShiftModifier
                                      __firstKeyRow = currentRow
                                  }
                              }

                              Keys.onReleased: {
                                  if (event.key === Qt.Key_Control)
                                  {
                                      console.log("onReleased ControlModifier");
                                      __modifiers &= !Qt.ControlModifier
                                  }

                                  if (event.key === Qt.Key_Shift)
                                  {
                                       console.log("onReleased ShiftModifier");
                                      __modifiers &= !Qt.ShiftModifier
                                      __firstKeyRow = -1
                                  }
                              }

                              MouseArea
                              {
                                  anchors.fill: parent
                                  acceptedButtons: /*Qt.LeftButton | */Qt.RightButton
                                  onClicked: {
                                      nodemanager.popup()
                                      console.log("Click2")
                                      if (mouse.button == Qt.RightButton)
                                      {
                  //                        if (__clicks && indexes.length > -1) lgh
                  //                        {
                  //                            console.log("Right")
                  //                             nodemanager.popup()
                  //                        }


                                      }
                                  }
                              }

                              onClicked:
                              {
                                  if (1 == __isfullsel || 2 == __isfullsel)
                                  {
                                      __isfullsel = 0;
                                      __modifiers = 0;
                                  }

                  //                __modifiers = 0;
                                  var index = currentRow
                                  clickrowinfo = currentRow;

                                  if (__modifiers & Qt.ShiftModifier)
                                  {
                  //                    console.log("currentRo111w="+currentRow)
                                      setRowRange(true, __previousRow, index)
                                  }
                                  else
                                  {
                  //                    console.log("currentRow22="+currentRow)
                                      var hasIndex = indexes.indexOf(index) !== -1
                                      if (__modifiers & Qt.ControlModifier) {
                                          console.log("currentRow333="+currentRow+" hasIndex="+hasIndex)
                                          setRowRange(!hasIndex, index, index)
                                      } else {
                                          console.log("currentRow444="+currentRow+" hasIndex="+hasIndex+" len="+indexes.length)
                                          indexes = (indexes.length == 1 && hasIndex) ? [] : [index]
                                          __clicks++  // force a re-evaluation of indexes in the delegates
                                      }
                                  }
                                  __previousRow = index
                              }



                              function setRowRange(select, first, last)
                              {
                                  var start = first <= last ? first : last
                                  var stop = first <= last ? last : first
                                  for (var i = start ; i <= stop ; ++ i)
                                  {

                                      if (__isfullsel == 2 && i === currentRow)
                                          continue
                                      var indexAt = indexes.indexOf(i)
                                      if (select)
                                      {
                                          if (indexAt < 0)
                                              indexes.push(i)
                                      } else
                                      {
                                          if (indexAt > -1)
                                              indexes.splice(indexAt, 1);
                                      }
                                  }
                                  __clicks++  // force a re-evaluation of indexes in the delegates
                              }

                              function setRowRange1(data)
                              {
                                  indexes = []

                                  var i=0;
                                  var j=0;
                                  var indexAt;
                                  for (i=0; i<ids.length; i++)
                                  {
                                      for (j=0; j<data.length; j++)
                                      {
                                          if (ids[i] === data[j])
                                          {
                                              indexAt = indexes.indexOf(data[j])
                                              if (indexAt < 0)
                                                  indexes.push(i)
                                              break;
                                          }
                                      }
                                  }

                                  for (i=0; i<ids.length; i++)
                                  {
                                      if (i === currentRow)
                                      {
                                          indexAt = indexes.indexOf(currentRow)
                                          if (indexAt < 0)
                                              indexes.push(i)
                                          break;
                                      }

                                  }

                                  __clicks++  // force a re-evaluation of indexes in the delegates
                              }

                      }

   }

   Component.onCompleted:
   {
          windowObj.visible = true
       var dir = "file:///"+appDir+"/gouxuan.png";

      //        for (var i=0; i<20; i++)
      //        {
      //            functionView1.append({"端口":"ttt.txt","类型":"332","代理账号":"33.33.33.33",
      //                                                  "代理密码":"11223"})
      //        }

               for (var i=0; i<20; i++)
               {
      //             libraryModel.append({"进程名":"ttt.txt","PID":"332","代理IP":"33.33.33.33",
      //                                                   "MAC":"11223","连接":"112", "发送":"223", "接收":"221",
      //                                                   "地理位置":"香港", "代理时长":"112", "comm":"112", "comm1":"112"})
               }
   }

}
