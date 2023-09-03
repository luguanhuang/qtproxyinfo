import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.0

Rectangle{
    id: userInfoPage
//    border.width: 5
//    border.color: "black"
    width: 300
    height: 120
    // 切换登录
    signal changePageToLogin()
    // 配置
    Settings {
        id: settings
        fileName: "./config.ini"
        property string userName
        property string userPass
    }
    Grid{
        id: userInfoList
        columns: 2
        rows : 4
        spacing: 10
        anchors.horizontalCenter:userInfoPage.horizontalCenter
        topPadding: 10
        Text{
            text: "用户名："
        }
        TextField {
            id: userName
            text:settings.userName
        }
        Text{
            text: "密码："
        }
        TextField {
            id: userPass
            text:settings.userPass
        }
        Text{
            text: "性别："
        }
        TextField {

        }
    }
    Row{
        spacing: 10
        anchors.bottom: userInfoPage.bottom
        anchors.horizontalCenter:userInfoPage.horizontalCenter
        Button{
            text:"更新"
            onClicked:{
                console.log(userName, userPass);
                settings.userName = userName.text
                settings.userPass = userPass.text
                changePageToLogin()
            }
        }
        Button{
            text:"退出登录"
            onClicked:{
                changePageToLogin();
            }
        }
    }

}
