import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.0

Rectangle{
    id: regUserPage
    width: 300
    height: 120
    // 切换登录
    signal changePageToLogin()
    // 退出信号
    signal appExit()
    // 配置
    Settings {
        id: settings
        fileName: "./config.ini"
        property string userName
        property string userPass
    }
    //
    Grid{
        id: regUserInfoList
        columns: 2
        rows : 4
        spacing: 10
        anchors.horizontalCenter:regUserPage.horizontalCenter
        topPadding: 10
        Text{
            text: "用户名："
        }
        TextField {
            id: userName
        }
        Text{
            text: "密码："
        }
        TextField {
            id: userPass
            echoMode: TextField.Password
        }
        Text{
            text: "性别："
        }
        TextField {

        }
    }
    Row{
        spacing: 10
        anchors.bottom: regUserInfoList.bottom
        anchors.horizontalCenter:regUserPage.horizontalCenter
        Button{
            x: 300; y: 40
            text:"注册"
            onClicked:{
                console.log(userName, userPass);
                settings.userName = userName.text
                settings.userPass = userPass.text
                changePageToLogin()
            }
        }
        Button{
            x: 300; y: 40
            text:"切换登录"
            onClicked:{
                changePageToLogin()
            }
        }
        Button{
            x: 500; y: 40
            text:"退出登录"
            onClicked:{
                // 发送退出信号
                appExit()
            }
        }
    }

}
