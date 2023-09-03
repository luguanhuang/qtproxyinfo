import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Ctrl14

import QtQuick.Controls.Styles 1.4

import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3

Window
{
    visible: true
    width: 420
    height: 560
     x:770
     y:270
    id:editprocess
    property int layout:2
    property int procid:0;
    title: qsTr("编辑进程")
    property variant containerqml: null

    Label
    {
        id: labelprocname
        x: 64; y: 10
        width: 80
        height: 20
        focus: true
        text: qsTr("进程名:") ;
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectprocname
        width: 240; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "black"
        anchors.top: parent.top; anchors.topMargin: 7
        anchors.left: parent.left; anchors.leftMargin: 110
        TextInput
        {
            id: inputprocname
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
            color: "black"
       }
    }

    Image {
        anchors.top: parent.top; anchors.topMargin: 7
        anchors.left: parent.left; anchors.leftMargin: 110+240+20
//        x: 20; y: 20
        source: "file:///"+"F:\\code\\assign\\chinese\\proxyserver\\newproxyinfo\\build-newproxyinfo-Desktop_Qt_5_15_2_MSVC2019_32bit-Release\\release\\"+"NoxVMHandle_icon.png"
    }

    Label
    {
        id: labelmd5
        anchors.top: labelprocname.top; anchors.topMargin: 35
        anchors.left: labelprocname.left; anchors.leftMargin: 20

        width: 80
        height: 20
        focus: true
        text: qsTr("MD5:") ;
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectmd5
        width: 250; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectprocname.top; anchors.topMargin: 34
        anchors.left: rectprocname.left; anchors.leftMargin: 0
        TextInput
        {
            id: textmd5
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Label
    {
        id: labelpath
        anchors.top: labelmd5.top; anchors.topMargin: 35
        anchors.left: labelmd5.left; anchors.leftMargin: -5

        width: 80
        height: 20
        focus: true
        text: qsTr("路径:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectpath
        width: 300; height: 50;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectmd5.top; anchors.topMargin: 34
        anchors.left: rectmd5.left; anchors.leftMargin: 0
        TextInput
        {
            id: textpath
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 8
            focus: true
            wrapMode: Text.WordWrap //换行
       }
    }

    Label
    {
        id: labelproxytype
        anchors.top: labelpath.top; anchors.topMargin: 55
        anchors.left: labelpath.left; anchors.leftMargin: -25

        width: 80
        height: 20
        focus: true
        text: qsTr("代理模式:") ;
        font.pointSize: 10
    }

    QtObject
    {
        id: backend
        property string modifier
    }

    ListModel
    {
       id: testModel
    }


    ComboBox
    {
        id: rectproxytype
        anchors.top: rectpath.top; anchors.topMargin: 54
        anchors.left: rectpath.left; anchors.leftMargin: 0

        width: 120
        height: 24;
        textRole: "text"
        onActivated: backend.modifier = currentValue
        currentIndex: 0
        Component.onCompleted:
        {
//            currentIndex = 0;
//            currentIndex = indexOfValue(backend.modifier)
        }
//        model: testModel
        model: [
        { value: Qt.ControlModifier, text: qsTr("TCP&UDP") }
        ]
//        model:ListModel{    //定义这个接口来动态添加下拉内容
//        id:lmd
//        }
        // model: ["First", "Second", "Third"]
   }

    Label
    {
        id: labelallocationstrategy
        anchors.top: labelproxytype.top;
        anchors.topMargin:
        {
            return 35
//            if (layout==2)
//                return 35
//            else
//                return 200
        }

        anchors.left: labelproxytype.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("分配策略:");
        font.pointSize: 10
    }

    ComboBox
    {
        id: allocationstrategy
        anchors.top: rectproxytype.top; anchors.topMargin: 34
        anchors.left: rectproxytype.left; anchors.leftMargin: 0

        width: 120
        height: 24;
        textRole: "text"
//        onActivated: backend.modifier = currentValue
//        currentIndex: 0
        Component.onCompleted:
        {

//            currentIndex = indexOfValue(backend.modifier)
        }

        model: [{ value: 0, text: qsTr("顺序") },
        { value: 1, text: qsTr("倒序") },
        { value: 2, text: qsTr("均衡") },
        { value: 3, text: qsTr("随机") },
        { value: 4, text: qsTr("绑模拟器") },
        { value: 5, text: qsTr("绑定窗口") },
        { value: 6, text: qsTr("依命令行") },
        ]

        onActivated:
         {
            console.log("currentText111="+currentText);
            if (currentText == "绑定窗口")
            {
                layout=1
                classname.visible = true;
                rectclassname.visible = true;

                beforeid.visible = true;
                rectbeforeid.visible = true;
                afterid.visible = true;
                rectafterid.visible = true;

                editprocess.height = 640

            }
            else if (currentText == "依命令行")
            {
                layout=1
                classname.visible = false;
                rectclassname.visible = false;

                beforeid.visible = true;
                rectbeforeid.visible = true;
                afterid.visible = true;
                rectafterid.visible = true;

                editprocess.height = 640

            }
            else
            {
                layout=2
                classname.visible = false;
                rectclassname.visible = false;

                beforeid.visible = false;
                rectbeforeid.visible = false;
                afterid.visible = false;
                rectafterid.visible = false;

                editprocess.height = 560
            }
        }
   }

    Label
    {
        id: classname
        anchors.top: labelproxytype.top; anchors.topMargin: 35
        anchors.left: labelproxytype.left; anchors.leftMargin: 185

        width: 80
        height: 20
        focus: true
        text: qsTr("类名:");
        font.pointSize: 10
        visible: false
    }

    //确认按钮
    Rectangle
    {
        id: rectclassname
        width: 140; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: labelproxytype.top; anchors.topMargin: 32
        anchors.left: labelproxytype.left; anchors.leftMargin: 220
        visible: false
        TextInput
        {
            id: textclassname
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Label
    {
        id: beforeid
        anchors.top: labelallocationstrategy.top;
        anchors.topMargin: 35
        anchors.left: labelallocationstrategy.left;
        anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("标识前:");
        font.pointSize: 10
        visible: false
    }

    //确认按钮
    Rectangle
    {
        id: rectbeforeid
        width: 280; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: allocationstrategy.top;
        anchors.topMargin: 32
        anchors.left: allocationstrategy.left;
        anchors.leftMargin: 0
        visible: false
        TextInput
        {
            id: textbeforeid
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Label
    {
        id: afterid
        anchors.top: beforeid.top; anchors.topMargin: 35
        anchors.left: beforeid.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("标识后:");
        visible: false
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectafterid
        width: 280; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectbeforeid.top; anchors.topMargin: 32
        anchors.left: rectbeforeid.left; anchors.leftMargin: 0
        visible: false
        TextInput
        {
            id: textafterid
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    CheckBox
    {

          id: proxysubproc
          anchors.top:
          {
              if(1 == layout)
              {
                rectafterid.top;
              }
              else
              {
                return allocationstrategy.top;
              }
          }

          anchors.topMargin: 25
          anchors.left:
          if(1 == layout)
          {
            rectafterid.left;
          }
          else
          {
            return allocationstrategy.left;
          }

          anchors.leftMargin: 0

          width: 80
          height: 20
          focus: true
          text: qsTr("代理子进程");
          checked: false
    }

    CheckBox
    {
          id: vmhardinfo
          anchors.top: proxysubproc.top; anchors.topMargin: 25
          anchors.left: proxysubproc.left; anchors.leftMargin: 0

          width: 90
          height: 20
          focus: true
          text: qsTr("虚拟硬件信息") ;
          checked: false
     }


    CheckBox
    {
          id: netlimit
          anchors.top: vmhardinfo.top; anchors.topMargin: 25
          anchors.left: vmhardinfo.left; anchors.leftMargin: 0

          width: 70
          height: 20
          focus: true
          text: qsTr("网速限制:") ;
          checked: false
    }

    Label
       {
           id: limitvalue
           anchors.top: vmhardinfo.top; anchors.topMargin: 25
           anchors.left: vmhardinfo.left; anchors.leftMargin: 130

           width: 80
           height: 20
           focus: true
           text: qsTr("KB/s = 0 125M/S = 1 Mbps");
           font.pointSize: 10
       }

    Rectangle
      {
          id: recnetlimit
          width: 50; height: 20;
          radius: 4
  //        color: "lightgrey"
          border.color: "grey"
          anchors.top: vmhardinfo.top; anchors.topMargin: 24
          anchors.left: vmhardinfo.left; anchors.leftMargin:70
          TextInput
          {
              id: textnetlimit
              anchors.fill: parent
              anchors.margins: 2
              font.pointSize: 10
              focus: true
              wrapMode: Text.WordWrap //换行
         }
      }

//    Label
//    {
//        id: limitvalue
//        anchors.top: vmhardinfo.top; anchors.topMargin: 30
//        anchors.left: vmhardinfo.left; anchors.leftMargin: 70

//        width: 80
//        height: 20
//        focus: true
//        text: qsTr("KB/s = 0 125M/S = 1 Mbps");
//        font.pointSize: 10
//    }

    Ctrl14.TabBar
    {
       id: bar
       width: 200
       height: 27
       visible: true
       anchors.top: limitvalue.top; anchors.topMargin: 30
       anchors.left: limitvalue.left; anchors.leftMargin: -190

       Ctrl14.TabButton
       {
          id: filter
           contentItem: Text
           {
                  text: filter.text
                  font: filter.font
                  opacity: enabled ? 1.0 : 0.3
                  color: bar.currentIndex == 0 ? "red" : "black"
//                    color: "black"
                  horizontalAlignment: Text.AlignHCenter
                  verticalAlignment: Text.AlignVCenter
                  elide: Text.ElideRight
           }

           background: Rectangle {
                  color: "white"
              }

           text: qsTr("过滤")
           onClicked:
           {

           }
       }

       Ctrl14.TabButton
       {
          id: dnsparser
          contentItem: Text {
                 text: dnsparser.text
                 font: dnsparser.font
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

       }
    }

    Label
    {
        id: labelfilterstatus
        anchors.top: bar.top; anchors.topMargin: 35
        anchors.left: bar.left; anchors.leftMargin: 20

        width: 80
        height: 20
        focus: true
        text: qsTr("过滤状态:");
        font.pointSize: 10
        visible: true
    }

    ComboBox
    {
        id: combofilterstatus
        anchors.top: bar.top; anchors.topMargin: 35
        anchors.left: bar.left; anchors.leftMargin: 80

        width: 120
        height: 24;
        textRole: "text"
//        onActivated: backend.modifier = currentValue
        Component.onCompleted:
        {

//            currentIndex = indexOfValue(backend.modifier)
        }

        model: [{ value: Qt.NoModifier, text: qsTr("不启用") },
        { value: Qt.ShiftModifier, text: qsTr("不过滤") },
        { value: Qt.ControlModifier, text: qsTr("仅过滤") }
        ]
//        model:ListModel{    //定义这个接口来动态添加下拉内容
//        id:lmd
//        }
        // model: ["First", "Second", "Third"]
   }

    Label
    {
        id: labeldestaddr
        anchors.top: labelfilterstatus.top; anchors.topMargin: 45
        anchors.left: labelfilterstatus.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("目标地址:");
        font.pointSize: 10
        visible: true
    }

    Rectangle
    {
        id: rectdestaddr
        x: 0; y: 0
        color: "#666666"
        anchors.top: combofilterstatus.top; anchors.topMargin: 45
        anchors.left: combofilterstatus.left; anchors.leftMargin: -2
        ScrollView
        {
            width: 250
            height: 160
            id: view11
            TextArea
            {
                width: 250
                height: 140
                id: destAddr
                wrapMode: Text.WordWrap //换行
            }
        }
    }

    Label
    {
        id: labelproxypasswd3
        anchors.top: labeldestaddr.top; anchors.topMargin: 25+160
        anchors.left: labeldestaddr.left; anchors.leftMargin: -20

        width: 80
        height: 20
        focus: true
        text: qsTr("不会影响已代理的进程,只对还未代理的进程有效");
        font.pointSize: 10
        visible: true
    }

    Button
    {
        id: btn_cancel
        width: 60
        height: 30
        text:"确定"
        anchors.top: labelproxypasswd3.top; anchors.topMargin: -10
        anchors.left: labelproxypasswd3.left; anchors.leftMargin: 290
        onClicked:
        {
            console.log("procid="+procid+" name="+inputprocname.text+" md5="+textmd5.text
                        +" textpath="+textpath.text+" currentIndex="+allocationstrategy.currentIndex
                        +" proxysubproc.checked="+proxysubproc.checked
                        +" vmhardinfo.checked="+vmhardinfo.checked+" netlimit="+netlimit.checked
                        +" textnetlimit="+textnetlimit.text+" allpc="+combofilterstatus.currentIndex
                        +" textArea="+destAddr.text)

            var resdata = func.editproc(String(procid)+"|"+inputprocname.text+"|"+textmd5.text
                               +"|"+textpath.text+"|"+allocationstrategy.currentIndex
                               +"|"+proxysubproc.checked+"|"+vmhardinfo.checked
                               +"|"+netlimit.checked+"|"+textnetlimit.text
                                             +"|"+combofilterstatus.currentIndex+"|"+destAddr.text
                                              +"|"+textclassname.text
                                                +"|"+textbeforeid.text
                                                +"|"+textafterid.text);

            resDialog.text = resdata;
            resDialog.open();
            console.log("resdata="+resdata)
        }
     }

    MessageDialog
    {
        id:resDialog
        title: ""
        text: ""
        icon: StandardIcon.Information;
        standardButtons:StandardButton.Ok;

        Component.onCompleted: visible = false

        onButtonClicked:
        {
            editprocess.close()
            containerqml.getprocinfo();//调用page1.qml中的函数，实现了传返回值。0
//            containerqml.getproxyinfo();//调用page1.qml中的函数，实现了传返回值。
        }
    }




    Component.onCompleted:
    {
        var test = func.getOneRowData(procid)
        var objinfo = JSON.parse(test);
        var str = "proc0";
        if (typeof(objinfo[str]) != "undefined")
        {
            inputprocname.text = objinfo[str].proc
            textmd5.text = objinfo[str].MD5
            textpath.text = objinfo[str].path
            if (objinfo[str].proxymode === '0')
            {
//                rectproxytype.currentIndex = 2;
            }

            allocationstrategy.currentIndex = parseInt(objinfo[str].stratery);
            if (5 == allocationstrategy.currentIndex)
            {
                layout=1
                classname.visible = true;
                rectclassname.visible = true;

                beforeid.visible = true;
                rectbeforeid.visible = true;
                afterid.visible = true;
                rectafterid.visible = true;

                editprocess.height = 640

                textclassname.text = objinfo[str].textclassname;
                textbeforeid.text = objinfo[str].textbeforeid
                textafterid.text = objinfo[str].textafterid
            }
            else if (6 == allocationstrategy.currentIndex)
            {
                layout=1
                beforeid.visible = true;
                rectbeforeid.visible = true;
                afterid.visible = true;
                rectafterid.visible = true;

                editprocess.height = 640

                textbeforeid.text = objinfo[str].textbeforeid
                textafterid.text = objinfo[str].textafterid
            }

            proxysubproc.checked = false;
            if (objinfo[str].subproc === 'true')
            {
                proxysubproc.checked = true;
            }

            vmhardinfo.checked = false;
            if (objinfo[str].virtual === 'true')
            {
                vmhardinfo.checked = true;
            }

            netlimit.checked = false;
            console.log("limit="+objinfo[str].limit)
            if (objinfo[str].limit === 'true')
            {
                netlimit.checked = true;
            }

            combofilterstatus.currentIndex=objinfo[str].filter

            textnetlimit.text = objinfo[str].limitval
            destAddr.text = objinfo[str].destaddr
//            destaddr

//            textnetlimit.text = "128"

//            console.log()


        //            var resdata = objinfo[str].ip + "|"+objinfo[str].port  + "|"+objinfo[str].type + "|"+objinfo[str].account
        //                     + "|"+objinfo[str].passwd + "|"+objinfo[str].pos + "|"+objinfo[str].line + "|"+objinfo[str].delay
        //             + "|"+objinfo[str].canassigned + "|"+objinfo[str].alreadyassigned + "|"+objinfo[str].deadline + "|"+objinfo[str].process
        //             + "|"+"启动" + "|"+objinfo[str].comment


//              var resdata = objinfo[str].ip + "|"+objinfo[str].port  + "|"+objinfo[str].type + "|"+objinfo[str].account
//                       + "|"+objinfo[str].passwd
//               + "|"+objinfo[str].canassigned + "|"+objinfo[str].alreadyassigned + "|"+objinfo[str].deadline + "|"+objinfo[str].process
//               + "|"+"启动" + "|"+objinfo[str].comment

//          console.log("resdata="+resdata);
//          textArea.text = resdata
        }
        else
        {

//          console.log("exist")
        }
     }

}

