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
    id:addproxy
    title: qsTr("添加代理")
    property variant containerqml: null

    Label
    {
        id: labelproxyip
        x: 64; y: 10
        width: 80
        height: 20
        focus: true
        text: qsTr("代理ip:") ;
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxyip
        width: 240; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "black"
        anchors.top: parent.top; anchors.topMargin: 7
        anchors.left: parent.left; anchors.leftMargin: 110
        TextInput
        {
            id: inputproxyip
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
            color: "black"
       }
    }

    Label
    {
        id: labelproxyport
        anchors.top: labelproxyip.top; anchors.topMargin: 35
        anchors.left: labelproxyip.left; anchors.leftMargin: -10

        width: 80
        height: 20
        focus: true
        text: qsTr("代理端口:") ;
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxyport
        width: 100; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxyip.top; anchors.topMargin: 34
        anchors.left: rectproxyip.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxyport
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Label
    {
        id: labelproxytype
        anchors.top: labelproxyport.top; anchors.topMargin: 35
        anchors.left: labelproxyport.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("代理类型:") ;
        font.pointSize: 10
    }

    QtObject
    {
        id: backend
        property string modifier
    }

    ComboBox
    {
        id: rectproxytype
        anchors.top: rectproxyport.top; anchors.topMargin: 34
        anchors.left: rectproxyport.left; anchors.leftMargin: 0

        width: 120
        height: 24;
        textRole: "text"
        onActivated: backend.modifier = currentValue
        Component.onCompleted:
        {

//            currentIndex = indexOfValue(backend.modifier)
        }

        model: [{ value: Qt.NoModifier, text: qsTr("SOCKS5") }
//        { value: Qt.ShiftModifier, text: qsTr("Shadowsocks") },
//        { value: Qt.ControlModifier, text: qsTr("Control") }
        ]
//        model:ListModel{    //定义这个接口来动态添加下拉内容
//        id:lmd
//        }
        // model: ["First", "Second", "Third"]
   }

    Label
    {
        id: labelproxyaccount
        anchors.top: labelproxytype.top; anchors.topMargin: 35
        anchors.left: labelproxytype.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("代理账号:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxyaccount
        width: 150; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxytype.top; anchors.topMargin: 34
        anchors.left: rectproxytype.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxyaccount
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Label
    {
        id: labelproxypasswd
        anchors.top: labelproxyaccount.top; anchors.topMargin: 35
        anchors.left: labelproxyaccount.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("代理密码:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxypasswd
        width: 150; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxyaccount.top; anchors.topMargin: 34
        anchors.left: rectproxyaccount.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxypasswd
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Button
    {
              id: rectproxydetect
              width: 50; height: 24;
      //        color: "lightgrey"
              anchors.top: rectproxyaccount.top; anchors.topMargin: 34
              anchors.left: rectproxypasswd.left; anchors.leftMargin: 160
//              Layout.margins: qGrid.cellmargin
              text:"检测"
              onClicked:
              {
                  console.log("inputproxyip="+inputproxyip.text);
                  labeltcpstatus.visible = true;

                  var retval = operProxy.checkSocks5Tcp(inputproxyip.text, textproxyport.text,
                                        textproxyaccount.text, textproxypasswd.text)
                  if (retval === 0)
                  {
                      labeltcpstatus.text = "    TCP正常.\n    开始检测UDP...";
                      retval = operProxy.checkSocks5Udp(inputproxyip.text, textproxyport.text,
                                                           textproxyaccount.text, textproxypasswd.text)
                      if (retval === 0)
                      {
                          labeltcpstatus.text = "    TCP正常.\n    UDP正常.";
//                        labeltcpstatus.text = "    UDP正常.";
                        var ipinfo = operProxy.getIpInfp(inputproxyip.text)
                         console.log("ipinfo="+ipinfo);
                          var arr = ipinfo.split('|');
                         console.log("a0="+arr[0]+" a1="+arr[1]+" a2="+arr[2]);
                        textproxypos.text = arr[2]+arr[3];
                          textproxyline.text = arr[4];
                      }
                      else
                      {
                        labeltcpstatus.text = "    检测结果: dial udp 0.0.0.0:0->\n"+inputproxyip.text+":"+textproxyport.text+": i/o timeout";
                      }
                  }
                  else
                  {
                        labeltcpstatus.text = "    检测结果: dial tcp 0.0.0.0:0->\n"+inputproxyip.text+":"+textproxyport.text+": i/o timeout";
                  }
              }
    }

    Label
    {
        id: labelproxypos
        anchors.top: labelproxypasswd.top; anchors.topMargin: 35
        anchors.left: labelproxypasswd.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("地理位置:");
        font.pointSize: 10
    }
//    id:test1111


    //确认按钮
    Rectangle
    {
        id: rectproxypos
        width: 210; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxypasswd.top; anchors.topMargin: 34
        anchors.left: rectproxypasswd.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxypos
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
            readOnly: true
       }
    }

    Button
    {
          id: rectproxyquery
          width: 50; height: 24;
  //        color: "lightgrey"
          anchors.top: rectproxypasswd.top; anchors.topMargin: 34
          anchors.left: rectproxypasswd.left; anchors.leftMargin: 220
//              Layout.margins: qGrid.cellmargin
          text:"查询"
          onClicked:
          {
            console.log("inputproxyip.text="+inputproxyip.text);
            var ipinfo = operProxy.getIpInfp(inputproxyip.text)
            console.log("ipinfo="+ipinfo);
            var arr = ipinfo.split('|');
            console.log("a0="+arr[0]+" a1="+arr[1]+" a2="+arr[2]);
            textproxypos.text = arr[2]+arr[3];
            textproxyline.text = arr[4];
          }
    }

    Label
    {
        id: labelproxyline
        anchors.top: labelproxypos.top; anchors.topMargin: 35
        anchors.left: labelproxypos.left; anchors.leftMargin: 25

        width: 80
        height: 20
        focus: true
        text: qsTr("线路:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxyline
        width: 250; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxypos.top; anchors.topMargin: 34
        anchors.left: rectproxypos.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxyline
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
            readOnly: true
       }
    }

    Label
    {
        id: labelproxydelay
        anchors.top: labelproxyline.top; anchors.topMargin: 35
        anchors.left: labelproxyline.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("延迟:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxydelay
        width: 50; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxyline.top; anchors.topMargin: 34
        anchors.left: rectproxyline.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxydelay
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
            readOnly: true
       }
    }

    Button
    {
        id: btnproxydelay
        width: 50
        height: 24
        text:"ping"
        anchors.top: rectproxyline.top; anchors.topMargin: 34
        anchors.left: rectproxyline.left; anchors.leftMargin: 60
        onClicked:
        {
            console.log("inputproxyip="+inputproxyip.text
                        +" textproxyport="+textproxyport.text+" textproxyaccount="+textproxyaccount.text
                        +" textproxypasswd="+textproxypasswd.text+" textproxypos="+textproxypos.text
                        +" textproxyline="+textproxyline.text+" textproxydelay="+textproxydelay.text
                        +" textproxyassignable="+textproxyassignable.text+" textproxydeadline="+textproxydeadline.text
                        +" textallowprocess="+textallowprocess.text+" textproxycomm="+textproxycomm.text);
        }
     }

    Label
    {
        id: labelproxyassignable
        anchors.top: labelproxydelay.top; anchors.topMargin: 29
        anchors.left: labelproxydelay.left; anchors.leftMargin: -13

        width: 80
        height: 20
        focus: true
        text: qsTr("可分配:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxyassignable
        width: 80; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxydelay.top; anchors.topMargin: 34
        anchors.left: rectproxydelay.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxyassignable
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

//    Datepicker {
//         id: myDate
//         activeWindow: main
//         width: 200
//     }

    Label
    {
        id: labeldeadline
        anchors.top: labelproxyassignable.top; anchors.topMargin: 34
        anchors.left: labelproxyassignable.left; anchors.leftMargin: -13

        width: 80
        height: 20
        focus: true
        text: qsTr("到期时间:");
        font.pointSize: 10
    }

    PageTimer
    {
//      id:pageLogTimeBegin
        id: rectproxydeadline
        //        width: 80; height: 24;
        //          radius: 4
        //          color: "lightgrey"
        //          border.color: "grey"
        anchors.top: rectproxyassignable.top; anchors.topMargin: 34
        anchors.left: rectproxyassignable.left; anchors.leftMargin: 0
        visible:true
    }

//    Rectangle {
//        visible: true
////        width: 400
////        height: 440
//        width: 200
//            height: 25
//        // 定时器：用于延时
//        Timer {
//            id: timer
//        }

//        id: rectproxydeadline
//      //        width: 80; height: 24;
////          radius: 4
////          color: "lightgrey"
////          border.color: "grey"
//          anchors.top: rectproxyassignable.top; anchors.topMargin: 34
//          anchors.left: rectproxyassignable.left; anchors.leftMargin: 0

////            x:120
////            y:260
//            z: 20 + 20

////            z: 300
//        // 计算两个时间之间的天数
//        function getRemainDay() {
//            var curDate = new Date();   // 当前日期
//            var endDate = calendar.selectedDate -12*60*60*1000 // 结束日期
//            var day = (endDate - curDate)/1000/60/60/24
//            if(day > -1.0)
//                return parseInt(day+1);
//            else
//                return parseInt(day);
//        }

//        // 延时函数


//        // MouseArea的层级在"其它所有控件"之下（因为写在"其它所有控件"的后面，又是同级的）
//        MouseArea{
//            anchors.fill: parent
//            onPressed: parent.forceActiveFocus() // 强制让窗口获得焦点
//        }

//        TextField {
//            id: textAreaEnd
//            width: 200
//            height: 25
//            placeholderText: "请选择日期"
//            readOnly: true // 只读
////            font.pointSize: 18
//            font.pointSize: 10
//            anchors.top: parent.top
//            anchors.topMargin: 30
//            anchors.horizontalCenter: parent.horizontalCenter
////            z: window.z + 10
//            // 检测焦点是否在文本输入框中，在则弹出"日历"
//            onFocusChanged: {
//                if (activeFocus)
//                    calendar.visible = true
//                else
//                    calendar.visible = false
//            }

//            // 避免选择日期后"日历"隐藏，焦点此时还在"文本框"上，无法进入"焦点改变事件"显示"日历"的情况
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    calendar.visible = true
//                    parent.forceActiveFocus()
//                }
//            }
//        }

//        // 日历控件
//        Calendar {
//            id: calendar
//            anchors.top: textAreaEnd.bottom
//            anchors.left: textAreaEnd.left
//            width: textAreaEnd.width
//            height: 320
//            visible: false
//            minimumDate: new Date()
////            z: test1111.z + 1
//            // 日历样式
//            style: CalendarStyle {
//                gridVisible: false // 网格不可见


//                // 设置日期的样式
//                dayDelegate: Rectangle {

//                    // 日期是否为今天
//                    property bool bIsToday: styleData.date.toLocaleString(Qt.locale("de_DE"), "yyyy-MM-dd hh:mm:ss") ===
//                                            (new Date()).toLocaleString(Qt.locale("de_DE"), "yyyy-MM-dd hh:mm:ss")

//                    gradient: Gradient {
//                        GradientStop {
//                            position: 0.00
//                            color: styleData.selected && styleData.date >= new Date() ? "SlateGray" : "white"
//                        }
//                    }

//                    Label {
//                        id: labDay
//                        text: styleData.date.getDate()
//                        font.family: "Microsoft YaHei"
//                        font.pixelSize: 16
//                        anchors.centerIn: parent
//                        color: (styleData.date > new Date() && styleData.selected) ? "white" :
//                               ((styleData.date > new Date() && styleData.visibleMonth)
//                               ? (bIsToday ? "blue" : "black") : "Silver")
//                    }
//                }
//            }

//            // 选择结束日期之后，隐藏日历
//            onClicked: {
//                 var now = new Date();
////                var dateString = Qt.formatDate(now, "hh:mm:ss");
//                var dateString = Qt.formatDateTime(new Date(), "hh-mm-ss");
//                textAreaEnd.text = Qt.formatDateTime(calendar.selectedDate, "yyyy-MM-dd") + " " +dateString

//                // 延时一会儿才隐藏日历（第二个参数为"函数"）
//                delay(200, function() {calendar.visible = false})
//            }
//        }
//    }

    //确认按钮
//    Rectangle
//    {
//        id: rectproxydeadline
//        width: 80; height: 24;
//        radius: 4
////        color: "lightgrey"
//        border.color: "grey"
//        anchors.top: rectproxyassignable.top; anchors.topMargin: 34
//        anchors.left: rectproxyassignable.left; anchors.leftMargin: 0
//        TextInput
//        {
//            id: textproxydeadline
//            anchors.fill: parent
//            anchors.margins: 2
//            font.pointSize: 10
//            focus: true

//       }
//    }


//    Calendar
//    {
//        anchors.top: rectproxyassignable.top; anchors.topMargin: 34
//        anchors.left: rectproxyassignable.left; anchors.leftMargin: 0
//        id:timeselector
//            visible:false
////              z: window.z + 100
//    }

    Label
    {
        id: labelallowproc
        anchors.top: rectproxydeadline.top; anchors.topMargin: 34
        anchors.left: rectproxydeadline.left; anchors.leftMargin: -95

        width: 80
        height: 20
        focus: true
        text: qsTr("允许使用的进程:");
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxyallowproc
        width: 240; height: 60;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: rectproxydeadline.top; anchors.topMargin: 34
        anchors.left: rectproxydeadline.left; anchors.leftMargin: 0
        ScrollView
        {
               id: view

               anchors.fill: parent
             TextArea {
                id: textallowprocess
                anchors.margins: 2
                font.pointSize: 10
                focus: true

               }
        }
    }

    Label
    {
        id: labelallowprochint
        anchors.top: rectproxyallowproc.top; anchors.topMargin: 65
        anchors.left: rectproxyallowproc.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("*ALLNAME;ALLMD5;标识此代理可以应用于全部\n非绑定的进程,字符段后必须添加\";\"");
        font.pointSize: 10
    }

    Label
    {
        id: labelstatus
        anchors.top: labelallowprochint.top; anchors.topMargin: 42
        anchors.left: labelallowprochint.left; anchors.leftMargin: -35

        width: 80
        height: 20
        focus: true
        text: qsTr("状态:");
        font.pointSize: 10
    }

    ComboBox
    {
        id: comboBoxproxystatus
        anchors.top: labelallowprochint.top; anchors.topMargin: 34
        anchors.left: labelallowprochint.left; anchors.leftMargin: 0

        width: 120
        height: 24;

        textRole: "text"
        onActivated: backend.modifier = currentValue
        Component.onCompleted:
        {

        }

        model: [{ value: Qt.NoModifier, text: qsTr("启用") },
        { value: Qt.ShiftModifier, text: qsTr("禁用") },
        { value: Qt.ControlModifier, text: qsTr("!启用")},
             { value: Qt.ShiftModifier, text: qsTr("!禁用") }
        ]
   }

    Label
    {
        id: labelproxycomm
        anchors.top: labelstatus.top; anchors.topMargin: 32
        anchors.left: labelstatus.left; anchors.leftMargin: 0

        width: 80
        height: 20
        focus: true
        text: qsTr("备注:") ;
        font.pointSize: 10
    }

    //确认按钮
    Rectangle
    {
        id: rectproxycomm
        width: 290; height: 24;
        radius: 4
//        color: "lightgrey"
        border.color: "grey"
        anchors.top: comboBoxproxystatus.top; anchors.topMargin: 34
        anchors.left: comboBoxproxystatus.left; anchors.leftMargin: 0
        TextInput
        {
            id: textproxycomm
            anchors.fill: parent
            anchors.margins: 2
            font.pointSize: 10
            focus: true
       }
    }

    Label
    {
        id: labeltcpstatus
        anchors.top: labelproxycomm.top; anchors.topMargin: 32
        anchors.left: labelproxycomm.left; anchors.leftMargin: -100

        width: 80
        height: 20
        focus: true
        text: qsTr("正在检测TCP,请等待...") ;
        font.pointSize: 10
        visible:false
    }

    MessageDialog
    {
        id:resDialog
        title: "返回框"
        text: "修改设备成功"
        icon: StandardIcon.Information;
        standardButtons:StandardButton.Ok;

        Component.onCompleted: visible = false

        onButtonClicked:
        {
            addproxy.close()
            containerqml.getproxyinfo();//调用page1.qml中的函数，实现了传返回值。
        }
    }

    Button
    {
        id: btn_cancel
        width: 70
        height: 30
        text:"添加"
        anchors.top: rectproxycomm.top; anchors.topMargin: 34
        anchors.left: rectproxycomm.left; anchors.leftMargin: 220
        onClicked:
        {
            if (rectproxydeadline.seltime == "")
            {
                var selectedDate = Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss")
                rectproxydeadline.seltime = selectedDate
            }

            console.log("inputproxyip="+inputproxyip.text
                        +" textproxyport="+textproxyport.text+" textproxyaccount="+textproxyaccount.text
                        +" textproxypasswd="+textproxypasswd.text+" textproxypos="+textproxypos.text
                        +" textproxyline="+textproxyline.text+" textproxydelay="+textproxydelay.text
                        +" textproxyassignable="+textproxyassignable.text+" seltime="+rectproxydeadline.seltime
                        +" textallowprocess="+textallowprocess.text+" textproxycomm="+textproxycomm.text);
            var resdata = operProxy.addProxy(inputproxyip.text+"|"+textproxyport.text
                               +"|"+textproxyaccount.text+"|"+textproxypasswd.text
                               +"|"+textproxyassignable.text+"|"+rectproxydeadline.seltime
                               +"|"+textallowprocess.text+"|"+textproxycomm.text);
            resDialog.text = resdata;
//            operProxy.myStringChanged(textArea.text);
            resDialog.open();
        }
     }


    Component.onCompleted:
    {
        textallowprocess.text = "ALLNAME;ALLMD5;"
     }

}

