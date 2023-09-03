import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Ctrl14

import QtQuick.Controls.Styles 1.4

import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3

//donot rename this entrance main.qml file, it will corrupt the project
Window
{
    visible: true
    width:  760
    height: 450
     x:770
     y:270

    title: qsTr("批量添加代理")
    property string textareainfo: ""
    property string assignableinfo: ""
    property string comm: ""
    property StackView stack: null
    property Component component: null
    property variant containerqml: null
    id:test

    onClosing:
    {
//        textArea.text = ""
//        textproxyassignable.text = "1"
//        textproxycomm.text = ""

    }

    Label
    {
        id: labelproxytext
        x: 0; y: 0
        width: 80
        height: 20
        focus: true
        text: qsTr("添加文本(需要添加的文本):") ;
        font.pointSize: 10
    }

    Label
    {
        id: labelproxyexample
        x: 380; y: 0
        width: 80
        height: 20
        focus: true
        text: qsTr("ip|端口|账号|密码|到期时间") ;
        font.pointSize: 10
    }


    Rectangle
    {
        id: textRect
        anchors.top: labelproxytext.top
        anchors.topMargin: 17
        anchors.left: labelproxytext.left
        anchors.leftMargin: 3

        color: "#666666"
        ScrollView
        {
            width: 750
            height: 160
            id: view11
            TextArea
            {
                width: 750
                height: 160
                id: textArea
            }
        }
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
                test.close()
                containerqml.getproxyinfo();//调用page1.qml中的函数，实现了传返回值。
            }
        }

        ComboBox
        {
            id: comboBoxproxystatus
              x: 600; y: 0
            width: 120
            height: 12;
            textRole: "text"
            onActivated:
            {

                if (currentText == "万安文件格式")
                {
                    labeldeadline.visible=false
                    rectproxydeadline.visible=false

                    console.log("currentText111="+currentText);
                }
                else if (currentText == "常用格式1")
                {
                    labeldeadline.visible=true
                    rectproxydeadline.visible=true
                    labelproxyexample.text = "ip|端口|账号|密码|到期时间";
                    console.log("currentText222="+currentText);
                }
                else if (currentText == "API格式")
                {
                    console.log("currentText333="+currentText);
                }
            }

            Component.onCompleted:
            {

            }

            model:
           [
                { value: 0, text: qsTr("万安文件格式") },
                { value: 1, text: qsTr("常用格式1") },
                 { value: 2, text: qsTr("API格式") },
            ]
       }

        Label
        {
            id: labelproxytype
            anchors.top: textRect.top; anchors.topMargin: 175
            anchors.left: textRect.left; anchors.leftMargin: 0

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
            anchors.top: textRect.top; anchors.topMargin: 170
            anchors.left: textRect.left; anchors.leftMargin: 60
            width: 120
            height: 24;

            textRole: "text"
            onActivated: backend.modifier = currentValue
            //Component.onCompleted: currentIndex = indexOfValue(backend.modifier)
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
            id: labelproxyassignable
            anchors.top: labelproxytype.top; anchors.topMargin: 29
            anchors.left: labelproxytype.left; anchors.leftMargin: 12

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
            anchors.top: rectproxytype.top; anchors.topMargin: 34
            anchors.left: rectproxytype.left; anchors.leftMargin: 0
            TextInput
            {
                id: textproxyassignable
                anchors.fill: parent
                anchors.margins: 2
                font.pointSize: 10
                focus: true
           }
        }

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
            anchors.top: rectproxyassignable.top; anchors.topMargin:32
            anchors.left: rectproxyassignable.left; anchors.leftMargin: 0
//            visible:true
            visible:false
        }

        Label
        {
            id: labelproxycomm
            anchors.top: labeldeadline.top; anchors.topMargin: 32
            anchors.left: labeldeadline.left; anchors.leftMargin: 6

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
            anchors.top: rectproxydeadline.top; anchors.topMargin: 34
            anchors.left: rectproxydeadline.left; anchors.leftMargin: 0
            TextInput
            {
                id: textproxycomm
                anchors.fill: parent
                anchors.margins: 2
                font.pointSize: 10
                focus: true
           }
        }

     CheckBox
    {

      id: checkaval
      anchors.top: labelproxycomm.top; anchors.topMargin: 40+70
      anchors.left: labelproxycomm.left; anchors.leftMargin:500

      checked: false
      text: qsTr("自动检测可用性")
      onCheckedChanged:
      {
          if (checkaval.checked === true)
          {
              console.log("check");
          }
          else
          {
                console.log("uncheck");
          }
      }
    }

     Rectangle
     {
        id: confirmButtonRect87
        width: 100
        height: 40
        color: "#FF5362"
        radius: 4
        anchors.top: labelproxycomm.top; anchors.topMargin: 25+70
        anchors.left: labelproxycomm.left; anchors.leftMargin:620

//        WorkerScript
//        {
//           id: myWorker
//           source: "script.mjs"
//           onMessage:
//           {
//               //myText.text = messageObject.reply //用于接收 mjs 发出的信号
//               console.log("recv data="+messageObject.reply)
//           }

//        }

        Button{
        id: findText
        width: parent.width; height: 40
        //Layout.margins:  qGrid.cellmargin
        text:qsTr("确定")
        onClicked:
        {
            var resdata = operProxy.batAddProxy(textArea.text, textproxyassignable.text, textproxycomm.text);
            resDialog.text = resdata;
//            operProxy.myStringChanged(textArea.text);
            resDialog.open();
        }
     }
    }



     Component.onCompleted:
     {
//         console.log("on complete")
         textproxyassignable.text = "1"
         textproxycomm.text = ""
//         format.checked = true;
//         formatparse.checked = true;
         checkaval.checked = true;
         labeldeadline.visible=false
         rectproxydeadline.visible=false
        labelproxyexample.text = "ip|端口|账号|密码|到期时间";

      }
}

