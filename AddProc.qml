import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Ctrl14

import QtQuick.Controls.Styles 1.4

import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import "common.js" as Common
//donot rename this entrance main.qml file, it will corrupt the project
Window
{
    visible: true
    width:  260
    height: 300
    x:610
    y:370

    title: qsTr("添加进程")
    property int angle: 0;
    property int proxyid:0;
    property string textareainfo: "";
    property string assignableinfo: "";
    property string comm: ""
    property variant containerqml: null

    id:addproc
    Rectangle
    {
        id: textRect
        x: 10; y: 0
        color: "#666666"
        width: 240
        height: 160

        TextArea
        {
            width: 240
            height: 160
            id: textArea
        }

    }

    MessageDialog
    {
        id:resDialog
        title: ""
        text: "添加进程成功"
        icon: StandardIcon.Information;
        standardButtons:StandardButton.Ok;

        Component.onCompleted: visible = false

        onButtonClicked:
        {
            textArea.text = ""
        }
    }

    Rectangle
    {
       id: confirmButtonRect87
       width: 100
       height: 40
       color: "#FF5362"
       radius: 4

       anchors.top: textRect.top; anchors.topMargin: 175
       anchors.left: textRect.left; anchors.leftMargin:100

       Button{
       id: findText
       width: parent.width; height: 40
       text:qsTr("确定")
       onClicked:
       {
           onTriggered:
            {
               var result = func.addProccess(textArea.text);
               if (0 === result)
               {
                   containerqml.getprocinfo();//调用page1.qml中的函数，实现了传返回值。

               }
//                console.log("test="+textArea.text)

            }
//           var resdata = func.addMultiProcInfo(textArea.text);
//           addproc.close()
//           containerqml.getprocinfo();//调用page1.qml中的函数，实现了传返回值。
       }
    }


   }

     Component.onCompleted:
     {

     }
}







