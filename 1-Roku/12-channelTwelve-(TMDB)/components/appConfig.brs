function configApp()
    items = 8
    uiResolution = getResolution()
    ratio = uiResolution.width/uiResolution.height
    itemWidth = uiResolution.width/items
    itemSpacing = ratio*10

    itemSize = {}
    itemSize.width = itemWidth
    itemSize.height = itemWidth*ratio

    rowListFields = {
        id: "rowList"
        translation: [ratio*20, ratio*20]
        rowItemSize: [[itemSize.width, itemSize.height]]
        itemSize: [uiResolution.width, itemSize.height]
        numRows: 4
        itemComponentName: "baseItem"
        rowItemSpacing:[[itemSpacing, 0]]
        itemSpacing: [0, itemSpacing]
        rowFocusAnimationStyle: "fixedFocus"
        vertFocusAnimationStyle: "fixedFocus"
    }

    posterFields = {
        id: "poster"
        width: itemSize.width*0.95
        height: itemSize.height*0.75
    }

    rectangleFields = {
        id: "rectangle"
        width: itemSize.width*0.95
        height: itemSize.height*0.2
        color: "#101114"
    }

    labelFields = {
        width: itemSize.width*0.9
        font: fontType(20)
        ellipsizeOnBoundary: true,
    }

    baseLayoutFields = {
        id:"baseLayout"
        translation:[ratio*3, ratio*3]
        layoutDirection: "vert",
        itemSpacings:[itemSize.height*0.02]
    }

    metadataLayoutFields = {
        id:"metadataLayout"
        translation:[ratio*3, ratio*3]
        layoutDirection: "vert"
    }

    styles                      = {}
    styles.rowlistFields        = rowListFields
    styles.rectangleFields      = rectangleFields
    styles.posterFields         = posterFields
    styles.labelFields          = labelFields
    styles.baseLayoutFields     = baseLayoutFields
    styles.metadataLayoutFields = metadataLayoutFields

    logger = createObject("roSGNode", "logger")
    m.global.addFields({styles: styles, logger: logger})
end function

function fontType(fontSize)
    font = createObject("roSGNode", "font")
    font.uri = "font:BoldSystemFontFile"
    font.size = fontSize
    return font
end function

function getResolution()
    devInfo = createObject("roDeviceInfo")
    uiResolution = devInfo.GetUIResolution()
    print formatJson(uiResolution)
    return uiResolution
end function