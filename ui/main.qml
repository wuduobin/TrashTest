import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.15 as Controls215

Window {
    minimumWidth: 800
    maximumWidth: 800
    minimumHeight: 800
    maximumHeight: 800
    color: "ghostwhite"

    visible: true
    title: qsTr("温度补偿小工具")

    Calendar {

    }

    Controls215.Button {
        id: btnBegin
        x: 30
        y: 20
        width: 100

        contentItem: Text {
            text: "输出浓度"
            color: btnBegin.hovered ? "white" : "dodgerblue"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "微软雅黑"
            font.pixelSize: 14
        }

        background: Rectangle {
            radius: 18
            border.width: 1
            border.color:  btnBegin.hovered ? "transparent" : "dodgerblue"
            color: btnBegin.hovered ? (btnBegin.pressed ? "navy" : "dodgerblue") : "transparent"
        }

        onClicked: {
            console.log("btnBegin clicked.")
        }
    }

//    TableView{
//            id: phoneTable
//            x: 50
//            y: btnBegin.y + 10
//            focus: true

//            // TableViewColumn 描述表格的每一列
//            TableViewColumn{role: "name"; title: "Name"; width: 80; elideMode: Text.ElideRight;}
//            TableViewColumn{role: "cost"; title: "Cost"; width: 100;}
//            TableViewColumn{role: "manufacture"; title: "Manufacture"; width: 140;}

//            model: ListModel{
//                id: phoneModel

//                ListElement{
//                    name: "rongyao2";
//                    cost: "4900";
//                    manufacture: "huawei"
//                }

//                ListElement{
//                    name: "s6";
//                    cost: "4800";
//                    manufacture :"sumsung"
//                }

//                ListElement{
//                    name: "apple5"
//                    cost: "3300"
//                    manufacture: "apple"
//                }

//                ListElement{
//                    name: "Mi5"
//                    cost: "3200"
//                    manufacture: "xiaomi"
//                }
//            } // model is end
//        }
}
