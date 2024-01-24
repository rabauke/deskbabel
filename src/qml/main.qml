import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import DeskBabelQuick

ApplicationWindow {
    id: root

    readonly property int horizontalMargin: 12

    height: 720
    minimumHeight: 360
    minimumWidth: 480
    visible: true
    width: 480

    footer: ToolBar {
        height: 1.5 * label.height

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: horizontalMargin
            anchors.rightMargin: horizontalMargin

            Label {
                id: label
                Layout.fillWidth: true
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignLeft
                text: qsTr('%1: %2 translations').arg(application.currentDictionaryFile).arg(Number(application.dictionarySize).toLocaleString(Qt.locale(), 'f', 0))
                verticalAlignment: Qt.AlignVCenter
                visible: application.dictionaryReady
            }
        }
    }
    menuBar: MenuBar {
        id: menuBar
        Menu {
            id: fileMenu
            title: qsTr('File')

            MenuItem {
                enabled: application.dictionaryReady
                text: qsTr('Load dictionary')

                onTriggered: fileDialog.open()
            }
            MenuItem {
                text: qsTr('Close')

                onTriggered: root.close()
            }
        }
        Menu {
            id: dirMenu
            title: qsTr('Translation direction')
            enabled: application.dictionaryReady

            MenuItem {
                text: application.translationDirection0 + (application.translationDirection === AppModel.TranslationDirection.bidirectional ? '  ✓' : '')
                onTriggered: application.translationDirection = AppModel.TranslationDirection.bidirectional;
            }
            MenuItem {
                text: application.translationDirection1 + (application.translationDirection === AppModel.TranslationDirection.lang_a_to_b ? '  ✓' : '')
                onTriggered: application.translationDirection = AppModel.TranslationDirection.lang_a_to_b;
            }
            MenuItem {
                text: application.translationDirection2 + (application.translationDirection === AppModel.TranslationDirection.lang_b_to_a ? '  ✓' : '')
                onTriggered: application.translationDirection = AppModel.TranslationDirection.lang_b_to_a;
            }
        }
    }

    Component.onCompleted: {
        application.loadDefaultDictionary();
    }

    AppModel {
        id: application
        queryString: searchfield.text
    }
    TextField {
        id: searchfield
        anchors.left: parent.left
        anchors.leftMargin: horizontalMargin
        anchors.right: clearSign.left
        anchors.rightMargin: horizontalMargin
        anchors.top: parent.top
        anchors.topMargin: 12
        font.pixelSize: 18
        placeholderText: application.dictionaryReady ? qsTr('enter search') : ''
        readOnly: (!application.dictionaryReady) || (application.dictionarySize === 0)
        text: ''
    }
    Text {
        id: clearSign
        anchors.verticalCenter: searchfield.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: horizontalMargin
        color: searchfield.text === '' ? searchfield.placeholderTextColor : searchfield.color
        font.pixelSize: 20
        text: '⨂'

        MouseArea {
            anchors.fill: parent

            onClicked: {
                searchfield.text = '';
            }
        }
    }
    ListView {
        id: translationsList
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: horizontalMargin
        anchors.right: parent.right
        anchors.top: searchfield.bottom
        anchors.topMargin: 12
        clip: true
        model: application.translations
        section.property: 'category'

        ScrollBar.vertical: ScrollBar {
            id: scrollBar
            active: true
        }
        delegate: Item {
            id: item
            height: lang1.height + lang2.height + 3 + 3 + 4 + 4
            width: translationsList.width

            TextEdit {
                id: lang1
                anchors.bottomMargin: 3
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: scrollBar.width + horizontalMargin
                anchors.top: item.top
                anchors.topMargin: 4
                color: Material.foreground
                font.italic: false
                font.pixelSize: 14
                readOnly: true
                selectByMouse: true
                text: model.source
                wrapMode: TextEdit.Wrap
            }
            TextEdit {
                id: lang2
                anchors.bottomMargin: 4
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: scrollBar.width + horizontalMargin
                anchors.top: lang1.bottom
                anchors.topMargin: 3
                color: Material.foreground
                font.italic: true
                font.pixelSize: 14
                readOnly: true
                selectByMouse: true
                text: model.translation
                wrapMode: TextEdit.Wrap
            }
        }
        section.delegate: TextEdit {
            anchors.bottomMargin: 3
            anchors.topMargin: 3
            color: Material.accent
            font.italic: false
            horizontalAlignment: TextEdit.AlignHCenter
            readOnly: true
            text: section
            width: parent.width
            wrapMode: TextEdit.Wrap
        }
    }
    BusyIndicator {
        anchors.centerIn: parent
        running: !application.dictionaryReady
    }
    FileDialog {
        id: fileDialog
        currentFolder: application.currentDictionaryFile
        modality: Qt.WindowModal
        title: qsTr('Please choose a dictionary')

        onAccepted: {
            searchfield.text = '';
            application.loadDictionary(fileDialog.selectedFile);
        }
    }
}
