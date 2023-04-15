import QtQuick

Item {
    property url currentFolder: "file://"
    property bool dictionaryReady: true
    property int dictionarySize: 12345
    property string queryString: "Hallo"
    property QtObject translations: ListModel {
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source source"
            translation: "translation translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source source source"
            translation: "translation translation translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source source source source"
            translation: "translation translation translation translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source source source source source"
            translation: "translation translation translation translation translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source source source source source"
            translation: "translation translation translation translation translation translation translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang1 -> Lang2"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source source"
            translation: "translation translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source source source"
            translation: "translation translation translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source source source source"
            translation: "translation translation translation translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source source source source source"
            translation: "translation translation translation translation translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source source source source source"
            translation: "translation translation translation translation translation translation translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
        ListElement {
            category: "Lang2 -> Lang1"
            source: "source source"
            translation: "translation translation"
        }
    }

    function loadDefaultDictionary() {
    }
    function loadDictionary(url) {
    }
}
