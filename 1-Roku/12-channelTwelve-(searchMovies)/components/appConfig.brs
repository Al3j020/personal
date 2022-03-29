function configApp()
    addLogger()
    items = 8
    uiResolution = getResolution()
    ratio = uiResolution.width/uiResolution.height
    itemWidth = uiResolution.width/items

    itemSize = {}
    itemSize.width = itemWidth
    itemSize.height = itemWidth*ratio

    mainScene = {id: "mainScene", backgroundColor: "#212F3C", backgroundUri: ""}

    baseScene = {
        rowListFields : {
            id: "rowList"
            translation: [3.25*itemSize.width , uiResolution.height*0.1]
            rowItemSize: [[itemSize.width, itemSize.height]]
            itemSize: [5*(itemSize.width + itemSize.width*0.05), itemSize.height]
            numRows: 4
            itemComponentName: "baseItem"
            rowItemSpacing:[[itemSize.width*0.05, 0]]
            itemSpacing: [0, itemSize.height*0.05]
            rowFocusAnimationStyle: "floatingFocus"
            vertFocusAnimationStyle: "fixedFocus"
        }

        multiStylelabelFields : {
            translation: [uiResolution.width*0.025, uiResolution.height*0.025]
            text: "Roku | Search Movies Channel | Alejandro Gutiérrez Franco"
            drawingStyles : {"default": {"fontSize": 35, "fontUri": "font:BoldSystemFontFile", "color": "#FFFFFF"}}
        }

        keyboardFields : {
            id: "keyboard"
            translation: [uiResolution.width*0.01, uiResolution.height*0.1]
            opacity: 0.75
        }
    }

    baseItem = {
        posterFields : {
            id: "poster"
            width: itemSize.width*0.95
            height: itemSize.height*0.75
        }

        rectangleFields : {
            id: "rectangle"
            width: itemSize.width*0.95
            height: itemSize.height*0.2
            color: "#3498DB"
        }

        labelFields : {
            width: itemSize.width*0.9
            font: fontType(20)
            ellipsizeOnBoundary: true,
        }

        baseLayoutFields : {
            id:"baseLayout"
            translation:[ratio*3, ratio*3]
            layoutDirection: "vert",
            itemSpacings:[itemSize.height*0.02]
        }

        metadataLayoutFields : {
            id:"metadataLayout"
            translation:[ratio*3, ratio*3]
            layoutDirection: "vert"
        }
    }

    styles                      = {}
    styles.mainScene            = mainScene
    styles.baseScene            = baseScene
    styles.baseItem             = baseItem

    m.global.addFields({styles: styles})
end function

sub addLogger()
    m.global.addFields({logger: createObject("roSGNode", "logger")})
end sub

function fontType(fontSize)
    font = createObject("roSGNode", "font")
    font.uri = "font:BoldSystemFontFile"
    font.size = fontSize
    return font
end function

function getResolution()
    devInfo = createObject("roDeviceInfo")
    uiResolution = devInfo.GetUIResolution()
    m.global.logger.callFunc("consoleLog", {info: uiResolution})
    return uiResolution
end function