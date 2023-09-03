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
    width:  360
    height: 120
    x:720
    y:420

    title: qsTr("选中到期时间")
    property int angle: 0;
    property int proxyid:0;
    property string textareainfo: "";
    property string assignableinfo: "";
    property string comm: ""
    property int listdata: 0
    property variant containerqml: null

    id:test11

//    onClosing:
//    {
//        console.log("on closing");
////        winmain.stackView11.pop()
////        stack.pop()
//    //        textArea.text = ""
//    //        textproxyassignable.text = "1"
//    //        textproxycomm.text = ""
////    test.hide()

//    }

    Rectangle
    {
        id: textRect
//    anchors.top: labelproxytext.top
//    anchors.topMargin: 17
//    anchors.left: labelproxytext.left
//    anchors.leftMargin: 3
    x: 5; y: 5
//    color: "#666666"
    color: "#20293c"
        width: 345
        height: 60
//        id: view11
        TextArea
        {
            width: 345
            height: 60
            id: textArea
            readOnly: true
        }

    }

    TextField
    {
        id: deadline
//        x: 136
//        y: 510
        width: 345
        height: 20

        //                        wrapMode: Text.Wrap
        selectByMouse: true
        //                        color: "#20293c"
//        placeholderText: qsTr("input description text")
        anchors.top: textRect.top; anchors.topMargin: 65
        anchors.left: textRect.left; anchors.leftMargin:2

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
//            textArea.text = ""
//            textproxyassignable.text = "1"
//            textproxycomm.text = ""
//            test.hide()
//            textareainfo = 1
            console.log("will call close function")
            test11.close()
            containerqml.getproxyinfo();//调用page1.qml中的函数，实现了传返回值。
        }
    }

    Rectangle
    {
       id: confirmButtonRec
       width: 80
       height: 20
       color: "#FF5362"
       radius: 4

       anchors.top: textRect.top; anchors.topMargin: 90
       anchors.left: textRect.left; anchors.leftMargin:640-550

       Button{
       id: confirmText
       width: parent.width; height: 20
       //Layout.margins:  qGrid.cellmargin
       text:qsTr("确定输入(O)")
       onClicked:
       {
           var strids = operProxy.getseldeadline(deadline.text)
           var resdata = strids.split(",");

           var len = resdata.length;

            if (len > 0)
            {
                len = len - 1;
            }

            var hint = "匹配到并选中"+String(len)+"个[符合]条件"+deadline.text+"的到期时间"

            resDialog.text = hint;
            resDialog.open()

            containerqml.selecgids = strids
            containerqml.selectfunction();//调用page1.qml中的函数，实现了传返回值。
       }
    }
   }

    Rectangle
    {
       id: cancelButtonRec
       width: 80
       height: 20
       color: "#FF5362"
       radius: 4

       anchors.top: textRect.top; anchors.topMargin: 90
       anchors.left: textRect.left; anchors.leftMargin:640-450

       Button{
       id: cancelText
       width: parent.width; height: 20
       //Layout.margins:  qGrid.cellmargin
       text:qsTr("取消(C)")
       onClicked:
       {
//            console.log("on edit complete textArea.text="+textArea.text)
//           operProxy.updateProxyConf(proxyid, textArea.text)
           close();

       }
    }


   }

     Component.onCompleted:
     {
         console.log(Qt.formatDateTime(new Date(),"yyyy-MM-dd HH:mm:ss.zzz"))
        textArea.text = "请输入到期时间";
         deadline.text = Qt.formatDateTime(new Date(),"yyyy-MM-dd HH:mm:ss")

      }
}







