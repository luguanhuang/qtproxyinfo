import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Ctrl14

import QtQuick.Controls.Styles 1.4

import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3


Rectangle
{
    property string seltime: ""
    visible: true
//        width: 400
//        height: 440
    width: 200
        height:25
    // 定时器：用于延时
    Timer {
        id: timer
    }

    z: 3

    function delay(delayTime, cb) {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.triggered.connect(cb);
        timer.start();
    }

//            z: 300
    // 计算两个时间之间的天数
    function getRemainDay() {
        var curDate = new Date();   // 当前日期
        var endDate = calendar.selectedDate -12*60*60*1000 // 结束日期
        var day = (endDate - curDate)/1000/60/60/24
        if(day > -1.0)
            return parseInt(day+1);
        else
            return parseInt(day);
    }

    // 延时函数


    // MouseArea的层级在"其它所有控件"之下（因为写在"其它所有控件"的后面，又是同级的）
    MouseArea{
        anchors.fill: parent
        onPressed: parent.forceActiveFocus() // 强制让窗口获得焦点
    }

    TextField {
        id: textAreaEnd
        width: 200
        height: 25
//        Text: Qt.formatDateTime(new Date(), "yyyy-MM-dd hh-mm-ss")
        placeholderText: Qt.formatDateTime(new Date(), "yyyy-MM-dd hh-mm-ss")
        readOnly: true // 只读
//            font.pointSize: 18
        font.pointSize: 10
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
//            z: window.z + 10
        // 检测焦点是否在文本输入框中，在则弹出"日历"
        onFocusChanged: {
            if (activeFocus)
                calendar.visible = true
            else
                calendar.visible = false
        }

        // 避免选择日期后"日历"隐藏，焦点此时还在"文本框"上，无法进入"焦点改变事件"显示"日历"的情况
        MouseArea {
            anchors.fill: parent
            onClicked: {
                calendar.visible = true
                parent.forceActiveFocus()
            }
        }
    }

    // 日历控件
    Calendar {
        id: calendar
        anchors.top: textAreaEnd.bottom
        anchors.left: textAreaEnd.left
        width: textAreaEnd.width
//        height: 320
          height: 150
        visible: false
        minimumDate: new Date()
//            z: test1111.z + 1
        // 日历样式
        style: CalendarStyle {
            gridVisible: false // 网格不可见


            // 设置日期的样式
            dayDelegate: Rectangle {

                // 日期是否为今天
                               property bool bIsToday: styleData.date.toLocaleString(Qt.locale("de_DE"), "yyyy-MM-dd") ===
                                                       (new Date()).toLocaleString(Qt.locale("de_DE"), "yyyy-MM-dd")

                gradient: Gradient {
                    GradientStop {
                        position: 0.00
                        color: styleData.selected && styleData.date >= new Date() ? "SlateGray" : "white"
                    }
                }

                Label {
                    id: labDay
                    text: styleData.date.getDate()
                    font.family: "Microsoft YaHei"
                    font.pixelSize: 16
                    anchors.centerIn: parent
                    color: (styleData.date > new Date() && styleData.selected) ? "white" :
                           ((styleData.date > new Date() && styleData.visibleMonth)
                           ? (bIsToday ? "blue" : "black") : "Silver")
                }
            }
        }

        // 选择结束日期之后，隐藏日历
        onClicked: {
             var now = new Date();
//                var dateString = Qt.formatDate(now, "hh:mm:ss");
            var dateString = Qt.formatDateTime(new Date(), "hh-mm-ss");
            textAreaEnd.text = Qt.formatDateTime(calendar.selectedDate, "yyyy-MM-dd") + " " +dateString
            seltime = textAreaEnd.text
            // 延时一会儿才隐藏日历（第二个参数为"函数"）
            delay(200, function() {calendar.visible = false})
        }
    }

//    Component.onCompleted:
//    {
//        console.log("on complete 111")
//     }
}
