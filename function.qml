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
    id:functionView
    width: 1100    //+40
    height: 200       //+40
    property int procclickrowinfo:-1;

    x:0
    y:0
    signal entered();

    function cleardata()
    {
         libraryModel1.clear()
    }

    function addData(proc, MD5, path, proxymode, stratery, subproc, virtual, limit, filter)
    {
         libraryModel1.append({"进程名":proc,"MD5":MD5,"路径":path,
                                                           "代理模式":proxymode,"分配策略":stratery, "子进程":subproc, "虚拟":virtual,
                                                            "限速":limit, "过滤":filter})

        if (1 === __isfullsel || 2 === __isfullsel)
        {
            __isfullsel = 0;
            __modifiers = 0;
        }


        var index = currentRow


        if (__modifiers & Qt.ShiftModifier)
        {
            setRowRange(true, __previousRow, index)
        }
        else
        {
            var hasIndex = indexes.indexOf(index) !== -1
            if (hasIndex === true )
            {

                 procclickrowinfo = -1;
            }
            else
            {
                 procclickrowinfo = currentRow;
            }

            if (__modifiers & Qt.ControlModifier) {
//                console.log("88="+currentRow+" hasIndex="+hasIndex)
                setRowRange(!hasIndex, index, index)
            } else {
//                console.log("99="+currentRow+" hasIndex="+hasIndex+" len="+indexes.length)
                indexes = (indexes.length === 1 && hasIndex) ? [] : [index]
                __clicks++  // force a re-evaluation of indexes in the delegates
            }
        }
        __previousRow = index
    }


   clip: true
   visible: true

    TableViewColumn
    {
        role: "进程名"
        title: "进程名"
        width: 150

    }

    TableViewColumn
    {

        role: "MD5"
        title: "MD5"
        width: 100
    }
    TableViewColumn
    {
        role: "路径"
        title: "路径"
        width: 200

    }

    TableViewColumn
    {

        role: "代理模式"
        title: "代理模式"
        width: 80
    }
    TableViewColumn
    {
        role: "分配策略"
        title: "分配策略"
        width: 80

    }

    TableViewColumn
    {
        role: "子进程"
        title: "子进程"
        width: 70
    }

    TableViewColumn
    {
        role: "虚拟"
        title: "虚拟"
        width: 60
    }

    TableViewColumn
    {
        role: "限速"
        title: "限速"
        width: 100

    }

    TableViewColumn
    {
        role: "过滤"
        title: "过滤"
        width: 150
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

       procclickrowinfo = clickrow;
//       console.log("procclickrowinfo="+procclickrowinfo);
       if (shiftPressed) {
           indexes = []
           setRowRange(__modifiers & Qt.ShiftModifier, __firstKeyRow, row)
       } else {
           indexes = [row]
       }
   }

   function keySelect1(shiftPressed, row)
   {
       if (shiftPressed)
       {
           indexes = []
           __firstKeyRow = 0
           setRowRange(1, __firstKeyRow, row)
       } else {
           indexes = [row]
       }
   }

   Keys.onPressed: {
       if (event.key === Qt.Key_Control)
       {
//              console.log("33 onPressed ControlModifier");
              __modifiers |= Qt.ControlModifier
       }

       if (event.key === Qt.Key_Shift)
       {
//           console.log("44 onPressed ShiftModifier");
           __modifiers |= Qt.ShiftModifier
           __firstKeyRow = currentRow
       }
   }

   Keys.onReleased: {
       if (event.key === Qt.Key_Control)
       {
//           console.log("55 onReleased ControlModifier");
           __modifiers &= !Qt.ControlModifier
       }

       if (event.key === Qt.Key_Shift)
       {
//            console.log("66 onReleased ShiftModifier");
           __modifiers &= !Qt.ShiftModifier
           __firstKeyRow = -1
       }
   }

   MouseArea
   {
       anchors.fill: parent
       acceptedButtons: /*Qt.LeftButton | */Qt.RightButton
       onClicked: {
           procinfo.popup()
//           console.log("77Click2")
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
       if (1 === __isfullsel || 2 === __isfullsel)
       {
           __isfullsel = 0;
           __modifiers = 0;
       }

       var index = currentRow
//        console.log("currentRow22="+currentRow)

       if (__modifiers & Qt.ShiftModifier)
       {
//           console.log("currentRow23="+currentRow)
           setRowRange(true, __previousRow, index)
       }
       else
       {
//           console.log("currentRow24="+currentRow)
           var hasIndex = indexes.indexOf(index) !== -1
//           console.log("has="+hasIndex)
           if (hasIndex === true )
           {

                procclickrowinfo = -1;
           }
           else
           {
                procclickrowinfo = currentRow;
           }

           if (__modifiers & Qt.ControlModifier) {
//               console.log("88="+currentRow+" hasIndex="+hasIndex)
               setRowRange(!hasIndex, index, index)
           } else {
//               console.log("89="+currentRow+" hasIndex="+hasIndex)
//               console.log("99="+currentRow+" hasIndex="+hasIndex+" len="+indexes.length)
               indexes = (indexes.length === 1 && hasIndex) ? [] : [index]
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

           if (__isfullsel === 2 && i === currentRow)
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
