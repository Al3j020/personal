function configApp()
    addGlobalFields()
    m.uiResolution = getResolution()
    itemSize = getItemSizeDimensions(8, m.uiResolution , 10)

    m.ratio = m.uiResolution.width/m.uiResolution.height

    rowListFields = {
        id: "rowList"
        translation: [m.uiResolution.height*0.05, m.uiResolution.height*0.175]
        rowItemSize: [[itemSize.width, itemSize.height]]
        itemSize: [m.uiResolution.width, itemSize.height]
        numRows: 2
        itemComponentName: "baseItem"
        rowItemSpacing:[[itemSize.itemSpacing, 0]]
        itemSpacing: [0, itemSize.itemSpacing]
        drawFocusFeedback: true
        rowFocusAnimationStyle: "fixedFocusWrap"
        vertFocusAnimationStyle: "fixedFocus"
    }

    baseItemFields = {
        posterFields : {
            id: "poster"
            width: itemSize.width*0.95
            height: itemSize.height*0.75
        }

        rectangleFields : {
            id: "rectangle"
            width: itemSize.width*0.95
            height: itemSize.height*0.2
            color: "#101114"
        }

        labelFields : {
            width: itemSize.width*0.9
            font: fontType(20)
            ellipsizeOnBoundary: true,
        }

        baseLayoutFields : {
            id:"baseLayout"
            translation:[m.ratio*3, m.ratio*3]
            layoutDirection: "vert",
            itemSpacings:[itemSize.height*0.02]
        }

        metadataLayoutFields : {
            id:"metadataLayout"
            translation:[m.ratio*3, m.ratio*3]
            layoutDirection: "vert"
        }
    }

    genres = [
        {"genreId": 0,  "genre": "All"},
        {"genreId": 18, "genre": "Drama"},
        {"genreId": 16, "genre": "Animation"},
        {"genreId": 35, "genre": "Comedy"},
        {"genreId": 80, "genre": "Crime"},
        {"genreId": 99, "genre": "Documentary"},
    ]

    topBarItemSize = getItemSizeDimensions(genres.count() + 1, m.uiResolution, 0)
    topBarRowSize = [m.uiResolution.width*0.8, topBarItemSize.height*0.175]

    topBarFields = {
        id: "topBar"
        translation: [(m.uiResolution.width - topBarRowSize[0])/2, m.uiResolution.height*0.05]
        itemComponentName: "topItem"
        numRows: 1
        drawFocusFeedback: false
        itemSize: topBarRowSize
        rowItemSize: [[topBarItemSize.width*0.8, topBarItemSize.height*0.175]]
        content: createContent(genres)
    }

    topItemFields = {
        layoutFields : {itemSpacings: [0], layoutDirection:"vert"}
        labelFields : {width: topBarItemSize.width*0.8, height: topBarItemSize.height*0.15, horizAlign: "center", vertAlign: "center", font: fontType(30), color: "0x7C7C7C"}
        rectangleFields : {width: topBarItemSize.width*0.8, height: topBarItemSize.height*0.025, color: "#0261fa", visible : false}
    }

    x = (m.uiResolution.width - (itemSize.width*1.75 + 20 + m.uiResolution.width/2))/2
    y = m.uiResolution.height*0.05
    detailsSceneFields = {
        posterFields : {id: "poster", width: itemSize.width*1.5, height: itemSize.width*1.5*m.ratio}
        buttonGroupFields : {id: "buttonGroup",  minWidth: m.uiResolution.width/2, textFont: fontType(35), focusedTextFont: fontType(35), iconUri: "", focusedIconUri: "", buttons: ["> Play", "+ Add to favourites", "··· More like this"]}
        titleLabelFields : {id: "label", font: fontType(50)}
        descriptionlabelFields : {id: "label", width: m.uiResolution.width/2, wrap: true, font: fontType(30)}
        metadataLabelFields : {id: "descriptionlabel", width: m.uiResolution.width/2, font: fontType(25)}
        mainLayoutFields : {id: "mainLayout", translation: [x, y], layoutDirection: "vert", itemSpacings:[20]}
        baseLayoutFields : {id: "baseLayout", layoutDirection: "horiz", itemSpacings:[20]}
        metadataLayoutFields : {id: "metadataLayout", layoutDirection: "vert", itemSpacings: [20, 50]}
    }

    styles                      = {}
    styles.topBarFields         = topBarFields
    styles.topItemFields        = topItemFields
    styles.rowlistFields        = rowListFields
    styles.baseItemFields       = baseItemFields
    styles.detailsSceneFields   = detailsSceneFields

    m.global.styles = styles
end function

sub addGlobalFields()
    m.global.addField("styles", "assocarray", false)
	m.global.addField("logger", "node", false)
	m.global.addField("newSceneCTA", "assocarray", false)

    m.global.logger = createObject("roSGNode", "logger")
end sub

function getItemSizeDimensions(numItems, uiResolution, itemSpacingRatio)
    ratio = uiResolution.width/uiResolution.height
    itemSize = {}
    itemSize.width = uiResolution.width/numItems
    itemSize.height = (uiResolution.width/numItems)*ratio
    itemSize.itemSpacing = ratio*itemSpacingRatio
    return itemSize
end function

function getResolution()
    devInfo = createObject("roDeviceInfo")
    uiResolution = devInfo.getUIResolution()
    m.global.logger.callFunc("consoleLog", {info: uiResolution})
    return uiResolution
end function

function createContent(items)
    content = createObject("roSGNode", "contentNode")
    rowContent = content.createChild("contentNode")
    for each item in items
        itemContent = rowContent.createChild("contentNode")
        itemContent.update(item, true)
        if item.genre = "All"
            itemContent.update({genres: items}, true)
        end if
    end for
    return content
end function

function fontType(fontSize)
    font = createObject("roSGNode", "font")
    font.uri = "font:BoldSystemFontFile"
    font.size = (fontSize/1000)*m.uiResolution.height
    return font
end function