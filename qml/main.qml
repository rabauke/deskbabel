import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import DeskBabel.QmlComponents

ApplicationWindow {
  id: root
  height: 720
  minimumHeight: 360
  minimumWidth: 480
  visible: true
  width: 480

  menuBar: MenuBar {
    id: menuBar
    Menu {
      id: fileMenu
      title: qsTr("File")

      MenuItem {
        text: qsTr("Load dictionary")

        onTriggered: fileDialog.open()
      }
      MenuItem {
        text: qsTr("Close")

        onTriggered: root.close()
      }
    }
  }

  Component.onCompleted: {
    application.load_default_dictionary();
  }

  AppModel {
    id: application
    queryString: searchfield.text
  }
  Item {
    anchors.bottomMargin: 8
    anchors.fill: parent
    anchors.leftMargin: 12
    anchors.rightMargin: 12
    anchors.topMargin: 8

    TextField {
      id: searchfield
      anchors.left: parent.left
      anchors.right: clear.left
      anchors.top: parent.top
      font.pixelSize: 18
      placeholderText: application.dictionaryReady ? qsTr('enter search') : ''
      readOnly: !application.dictionaryReady
      text: ''
    }
    Item {
      id: clear
      anchors.right: parent.right
      height: clearSign.height
      width: clearSign.width + 12

      Text {
        id: clearSign
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 6
        color: searchfield.text == '' ? searchfield.placeholderTextColor : searchfield.color
        font.pixelSize: 20
        text: '⨂'

        MouseArea {
          anchors.fill: parent

          onClicked: {
            searchfield.text = '';
          }
        }
      }
    }
    ListView {
      id: translationsList
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: searchfield.bottom
      clip: true
      model: application.translations
      section.property: 'category'

      delegate: Item {
        id: item
        height: lang1.height + lang2.height + 3 + 3 + 4 + 4
        width: translationsList.width

        TextEdit {
          id: lang1
          anchors.bottomMargin: 3
          anchors.top: item.top
          anchors.topMargin: 4
          color: Material.foreground
          font.italic: false
          font.pixelSize: 16
          readOnly: true
          selectByMouse: true
          text: model.source
          width: parent.width
          wrapMode: TextEdit.Wrap
        }
        TextEdit {
          id: lang2
          anchors.bottomMargin: 4
          anchors.top: lang1.bottom
          anchors.topMargin: 3
          color: Material.foreground
          font.italic: true
          font.pixelSize: 16
          readOnly: true
          selectByMouse: true
          text: model.translation
          width: parent.width
          wrapMode: TextEdit.Wrap
        }
      }
      section.delegate: TextEdit {
        anchors.bottomMargin: 3
        anchors.topMargin: 3
        color: Material.accent
        font.italic: false
        font.pixelSize: 16
        horizontalAlignment: TextEdit.AlignHCenter
        readOnly: true
        text: section
        width: parent.width
        wrapMode: TextEdit.Wrap
      }
    }
  }
  BusyIndicator {
    anchors.centerIn: parent
    running: !application.dictionaryReady
  }
  FileDialog {
    id: fileDialog
    currentFolder: StandardPaths.standardLocations(StandardPaths.AppDataLocation)[0]
    modality: Qt.WindowModal
    title: qsTr('Please choose a dictionary')

    onAccepted: {
      searchfield.text = '';
      application.load_dictionary(fileDialog.selectedFile);
    }
  }
}
