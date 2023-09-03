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
    height: 220
    x:610
    y:370

    title: qsTr("编辑代理")
    property int angle: 0;
    property int proxyid:0;
    property string textareainfo: "";
    property string assignableinfo: "";
    property string comm: ""


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
    x: 0; y: 0
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
            textArea.text = ""
            textproxyassignable.text = "1"
            textproxycomm.text = ""
            test.hide()
            textareainfo = 1
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
       anchors.left: textRect.left; anchors.leftMargin:640

       Button{
       id: findText
       width: parent.width; height: 40
       //Layout.margins:  qGrid.cellmargin
       text:qsTr("确定")
       onClicked:
       {
//            console.log("on edit complete textArea.text="+textArea.text)
           operProxy.updateProxyConf(proxyid, textArea.text)

       }
    }


   }

     Component.onCompleted:
     {
//        console.log("on edit complete proxyid="+proxyid)
        var test = operProxy.getOneRowData(proxyid)
//         console.log("on edit complete proxyid111="+proxyid)
         var objinfo = JSON.parse(test);
         var str = "proxy0";
        if (typeof(objinfo[str]) != "undefined")
        {
//            var resdata = objinfo[str].ip + "|"+objinfo[str].port  + "|"+objinfo[str].type + "|"+objinfo[str].account
//                     + "|"+objinfo[str].passwd + "|"+objinfo[str].pos + "|"+objinfo[str].line + "|"+objinfo[str].delay
//             + "|"+objinfo[str].canassigned + "|"+objinfo[str].alreadyassigned + "|"+objinfo[str].deadline + "|"+objinfo[str].process
//             + "|"+"启动" + "|"+objinfo[str].comment


                var resdata = objinfo[str].ip + "|"+objinfo[str].port  + "|"+objinfo[str].type + "|"+objinfo[str].account
                         + "|"+objinfo[str].passwd
                 + "|"+objinfo[str].canassigned + "|"+objinfo[str].alreadyassigned + "|"+objinfo[str].deadline + "|"+objinfo[str].process
                 + "|"+"启动" + "|"+objinfo[str].comment

            console.log("resdata="+resdata);
            textArea.text = resdata
        }
        else
        {

//            console.log("exist")
        }


      }
}







