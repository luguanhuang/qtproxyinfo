
import QtQuick 2.14
import QtQuick.Controls 2.14
import Qt.labs.qmlmodels 1.0
import QtGraphicalEffects 1.0

//import "../Component"

Item {
    id: mask
    width: parent.width
    height: parent.height

    property bool exportT: false

    function isChanged() {
        let count = table.model.rows.length
        for (var i = 0; i < count; i++) {
            if (table.model.rows[i].checked===1) {
                return true
            }
        }
        return false
    }

    Popup {
        id: popup
        width: 0
        height: 0
        modal: false
        padding: 0
        focus: true
        visible: true
        closePolicy: Popup.NoAutoClose
        Item {

            width: mask.width
            height: mask.height
            DropShadow {
                id: shadow
                anchors.fill: tmpId
                horizontalOffset: 5
                verticalOffset: 5
                radius: 8.0
                samples: 16
                color: "#80000000"
                source: tmpId
            }

            Rectangle {
                id: tmpId
                color: "white"
                z: 9
                width: dpH(1012)
                height: dpH(644)
                radius: dpH(10)

                x: Math.round((mask.width - width) / 2)
                y: Math.round((mask.height - height) / 2)
                visible: true

                Label {
                    id: tmp_top_label
                    text: qsTr("导入任务")
                    color: "#4A6DBC"
                    anchors {
                        top: parent.top
                        topMargin: dpH(21)
                        left: parent.left
                        leftMargin: dpH(47)
                    }
                    font {
                        bold: true
                        family: $font.name
                        pixelSize: dpH(23)
                    }
                }

                Rectangle {
                    z: 10
                    id: tableView
                    width: dpH(919)
                    height: dpH(475)
                    anchors {
                        top: parent.top
                        topMargin: dpH(88)
                        left: parent.left
                        leftMargin: dpH(47)
                    }

                    Item {
                        id: control
                        implicitHeight: parent.height
                        implicitWidth: parent.width

                        //表头行高
                        property int headerHeight: dpH(48)
                        //行高
                        property int rowHeight: dpH(48)
                        property int tableLeft: dpH(100)
                        //滚动条
                        property color scrollBarColor: "#E5E5E5"
                        property int scrollBarWidth: 7
                        //列宽
                        property variant columnWidthArr: [(control.width - control.tableLeft)
                            / 2, (control.width - control.tableLeft) / 2]

                        property var horHeader: ["名称", "格式"]
                        property int selected: -1
                        property var datas: [{
                                "checked": 0,
                                "id": 1,
                                "name": "汽车曲轴分拣项目1",
                                "type": "mission"
                            }, {
                                "checked": 0,
                                "id": 2,
                                "name": "汽车曲轴分拣项目2",
                                "type": "mission"
                            }, {
                                "checked": 0,
                                "id": 3,
                                "name": "汽车曲轴分拣项目3",
                                "type": "mission"
                            }, {
                                "checked": 0,
                                "id": 4,
                                "name": "汽车曲轴分拣项目4",
                                "type": "mission"
                            }, {
                                "checked": 0,
                                "id": 5,
                                "name": "汽车曲轴分拣项目5",
                                "type": "mission"
                            }, {
                                "checked": 0,
                                "id": 6,
                                "name": "汽车曲轴分拣项目6",
                                "type": "mission"
                            }]

                        //表格内容（不包含表头）
                        TableView {
                            id: table
                            anchors {
                                fill: parent
                                topMargin: control.rowHeight
                                leftMargin: control.tableLeft
                            }

                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            columnSpacing: 0
                            rowSpacing: 0

                            //此属性可以包含一个函数，该函数返回模型中每行的行高
                            rowHeightProvider: function (row) {
                                return control.headerHeight
                            }
                            //此属性可以保存一个函数，该函数返回模型中每个列的列宽
                            columnWidthProvider: function (column) {
                                return control.columnWidthArr[column]
                            }
                            ScrollBar.vertical: ScrollBar {
                                id: scroll_vertical
                                anchors.right: parent.right
                                anchors.rightMargin: 0
                                contentItem: Rectangle {
                                    visible: (scroll_vertical.size < 1.0)
                                    implicitWidth: control.scrollBarWidth
                                    color: control.scrollBarColor
                                }
                            }

                            ScrollBar.horizontal: ScrollBar {
                                id: scroll_horizontal
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 0
                                anchors.left: parent.left
                                anchors.leftMargin: -control.tableLeft
                                //active: table.ScrollBar.vertical.active
                                //policy: ScrollBar.AsNeeded
                                contentItem: Rectangle {
                                    visible: (scroll_horizontal.size < 1.0)
                                    implicitHeight: control.scrollBarWidth
                                    color: control.scrollBarColor
                                }
                            }

                            model: TableModel {

                                TableModelColumn {
                                    display: "name"
                                }
                                TableModelColumn {

                                    display: "type"
                                }

                                rows: control.datas
                            }

                            delegate: Rectangle {
                                color: table.model.rows.length > 0 && table.model.rows[row]
                                   && table.model.rows[row].checked===1 ? "#EAF1FF" :table.model.rows.length > 0 && table.model.rows[row]
                                                                          && table.model.rows[row].checked===2 ?"gray": "white"

                                Text {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    //elide: Text.ElideRight
                                    //获取单元格对应的值
                                    text: display
                                    font.pixelSize: dpH(15)
                                    color: "#707070"
                                }
                                Rectangle {
                                    color: "#E5E5E5"
                                    width: parent.width
                                    height: 1
                                    anchors {
                                        bottom: parent.bottom
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: table.model.rows.length > 0 && table.model.rows[row]
                                             &&table.model.rows[row].checked!==2? Qt.PointingHandCursor : Qt.ForbiddenCursor //悬浮或点击时的鼠标样式
                                    onClicked: {
                                            if(control.datas[row].checked===2){
                                                return false
                                            }
                                            if(control.datas[row].checked===1){
                                                control.datas[row].checked=0
                                            }else{
                                                control.datas[row].checked=1
                                            }


                                        table.model.rows = control.datas
                                        exportT = isChanged()
                                        if (checkBox.checked
                                                && !control.datas[row].checked) {
                                            checkBox.checked = false
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: control.tableLeft
                            height: control.rowHeight - 1
                            color: "#F8F8F8"
                            anchors {
                                top: parent.top
                                left: parent.left
                            }
                            CheckBoxButton {
                                property bool checked: false
                                id: checkBox
                                state: checked ? "checked" : "noCheck"
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    left: parent.left
                                    leftMargin: dpH(20)
                                }
                                onClicked: {
                                    checkBox.checked = !checkBox.checked
                                    let count = control.datas.length
                                    if (!count) {
                                        return false
                                    }

                                    for (var i = 0; i < count; i++) {
                                        if (checkBox.checked) {
                                            if(control.datas[i].checked===0){
                                                 control.datas[i].checked = 1
                                            }else if( control.datas[i].checked ===1){
                                                 control.datas[i].checked = 1
                                            }else{
                                                 control.datas[i].checked = 2
                                            }

                                        } else {
                                            if(control.datas[i].checked===0){
                                                 control.datas[i].checked = 0
                                            }else if( control.datas[i].checked ===1){
                                                 control.datas[i].checked = 0
                                            }else{
                                                 control.datas[i].checked = 2
                                            }
                                        }
                                    }
                                    table.model.rows = control.datas
                                    exportT = isChanged()
                                }
                            }
                            Text {
                                anchors {
                                    verticalCenter: checkBox.verticalCenter
                                    left: checkBox.right
                                    leftMargin: dpH(20)
                                }

                                text: checkBox.checked ? "取消" : "全选"
                                font.pixelSize: dpH(18)
                                color: "#707070"
                            }
                            Rectangle {
                                height: 1
                                width: parent.width
                                anchors.bottom: parent.bottom
                                color: "#E5E5E5"
                            }
                            Rectangle {
                                height: 1
                                width: parent.width
                                anchors.top: parent.top
                                color: "#E5E5E5"
                            }
                        }

                        Item {
                            anchors {
                                left: parent.left
                                right: parent.right
                                leftMargin: control.tableLeft
                            }
                            height: control.rowHeight
                            z: 2

                            Row {
                                anchors.fill: parent
                                leftPadding: -table.contentX
                                clip: true
                                spacing: 0

                                Repeater {
                                    model: table.columns > 0 ? table.columns : 0

                                    Rectangle {

                                        width: table.columnWidthProvider(
                                                   index) + table.columnSpacing
                                        height: control.rowHeight - 1
                                        color: "#F8F8F8"

                                        Text {
                                            anchors.centerIn: parent
                                            text: control.horHeader[index]
                                            font.pixelSize: dpH(18)
                                            color: "#707070"
                                        }
                                        Rectangle {
                                            height: 1
                                            width: parent.width
                                            anchors.bottom: parent.bottom
                                            color: "#E5E5E5"
                                        }
                                        Rectangle {
                                            height: 1
                                            width: parent.width
                                            anchors.top: parent.top
                                            color: "#E5E5E5"
                                        }
                                    }
                                }
                            }
                        }
                        Column {

                            anchors {
                                top: parent.top
                                bottom: parent.bottom
                                topMargin: control.headerHeight
                            }
                            topPadding: -table.contentY
                            z: 2
                            clip: true
                            spacing: 1
                            Repeater {
                                model: table.rows > 0 ? table.rows : 0
                                Rectangle {
                                    width: control.tableLeft
                                    height: table.rowHeightProvider(index)
                                    color: "white"
                                    Text{
                                         visible:table.model.rows.length > 0 && table.model.rows[index]
                                            &&table.model.rows[index].checked===2
                                        anchors.centerIn: parent
                                        font.pixelSize: dpH(14)
                                        color: "#4A6DBC"
                                        text:"使用中"
                                    }

                                    CheckBoxButton {
                                        visible:table.model.rows.length > 0
                                            && table.model.rows[index]
                                            &&table.model.rows[index].checked!==2
                                    state: table.model.rows.length > 0
                                           && table.model.rows[index]
                                           && table.model.rows[index].checked===1 ? "checked" : "noCheck"
                                        anchors.centerIn: parent

                                        onClicked: {
                                            if(control.datas[index].checked===2){
                                                return false
                                            }
                                            if(control.datas[index].checked===1){
                                                control.datas[index].checked=0
                                            }else{
                                                control.datas[index].checked=1
                                            }

                                            table.model.rows = control.datas
                                            exportT = isChanged()
                                            if (checkBox.checked
                                                    && !control.datas[index].checked) {
                                                checkBox.checked = false
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                NoBgButton {
                    id: btn_cancel
                    property bool btnStatus: true
                    z: 10

                    text: qsTr("取消")
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: dpH(24)
                        right: btn_ok.left
                        rightMargin: dpH(24)
                    }
                    onClicked: {

                        mask.destroy()
                    }
                }

                NoBgButton {
                    property bool btnStatus: exportT
                    z: 10
                    id: btn_ok
                    text: qsTr("导入")
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: dpH(24)
                        right: parent.right
                        rightMargin: dpH(46)
                    }
                    onClicked: {
                        if (btnStatus) {
//                            mask.destroy()
                            for (var k = 0; k < control.datas.length; k++) {
                                if (control.datas[k].checked===1) {
                                    control.datas[k].checked=2
                                }
                            }
                            table.model.rows = control.datas
                            exportT = isChanged()
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                }
            }
            MouseArea {
                id: mouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor //悬浮或点击时的鼠标样式
                onPressed: {
                    shadow.color = "#BC5555"
                }
                onReleased: {
                    shadow.color = "#80000000"
                }
            }
        }
    }
}

