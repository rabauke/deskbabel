import QtQuick

Item {
    property string queryString: "Hallo"
    property bool dictionaryReady: true
    property int dictionarySize: 12345
    property url currentFolder: "file://"

    property QtObject translations: DataEntryModel{}

    function loadDefaultDictionary() {}
    function loadDictionary(url) {}
}
