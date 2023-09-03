import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12 as Ctrl14

import QtQuick.Controls.Styles 1.4

import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3



TableView
{
    id:proxystatusView
    width: 1500    //+40
    height: 500       //+40
    property int procclickrowinfo:-1;

//    width: 600
//    height: 400
    x:0
    y:0
    signal entered();

    function cleardata()
    {
         libraryModel.clear()
    }

    function addData(proc, MD5, path, proxymode, stratery, subproc, virtual, dns, limit, filter)
    {
         libraryModel.append({"进程名":proc,"MD5":MD5,"路径":path,
                                                           "代理模式":proxymode,"分配策略":stratery, "子进程":subproc, "虚拟":virtual,
                                                           "域名解析":dns, "限速":limit, "过滤":filter})
//        console.log("111")
    }

   clip: true
   visible: true

    TableViewColumn
    {
        role: "进程名"
        title: "进程名"
        width: 180

    }

    TableViewColumn
    {

        role: "PID"
        title: "PID"
        width: 110
    }

    TableViewColumn
    {

        role: "代理IP"
        title: "代理IP"
        width: 160
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
//           console.log("Click11")
           if (mouse.button == Qt.RightButton)
           {
//                console.log("Click112222")
//                        if (__clicks && indexes.length > -1)
               {
//                   console.log("Right")
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


   function setRowRange(select, first, last)
   {
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

//Label
//{
//    id: labelhint
//    x: 64; y: 200
//    width: 80
//    height: 20
//    focus: true
//    text: qsTr("右键添加或将要代理的进程拖进下面:") ;
//    font.pointSize: 10
//}
