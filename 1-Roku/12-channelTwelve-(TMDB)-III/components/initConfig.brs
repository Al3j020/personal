function initConfig()
    addGlobalFields()
    m.uiResolution = getResolution()
    itemSize = getItemSizeDimensions(8, m.uiResolution , 10)
    m.ratio = m.uiResolution.width/m.uiResolution.height

    width  = m.uiResolution.width
    height = m.uiResolution.height

    grid = setupGrid(m.uiResolution)
    rowlistFields  = grid.rowlistFields
    baseItemFields = grid.itemFields

    menu = setupMenu(m.uiResolution)
    topBarFields = menu.rowlistFields
    topItemFields = menu.itemFields

    x = (m.uiResolution.width - (itemSize.width*1.75 + 20 + m.uiResolution.width/2))/2
    y = m.uiResolution.height*0.05

    detailsSceneFields = {
        topFields : {id: "detailsScene"}
        posterFields : {id: "poster", width: itemSize.width*1.5, height: itemSize.width*1.5*m.ratio}
        buttonGroupFields : {id: "buttonGroup",  minWidth: m.uiResolution.width/2, textFont: fontType(35), focusedTextFont: fontType(35), iconUri: "", focusedIconUri: "", buttons: ["> Play", "+ Add to favourites", "··· More like this"]}
        titleLabelFields : {id: "label", font: fontType(50)}
        descriptionlabelFields : {id: "label", width: m.uiResolution.width/2, wrap: true, font: fontType(30)}
        metadataLabelFields : {id: "descriptionlabel", width: m.uiResolution.width/2, font: fontType(25)}
        mainLayoutFields : {id: "mainLayout", translation: [x, y], layoutDirection: "vert", itemSpacings:[20]}
        baseLayoutFields : {id: "baseLayout", layoutDirection: "horiz", itemSpacings:[20]}
        metadataLayoutFields : {id: "metadataLayout", layoutDirection: "vert", itemSpacings: [20, 50]}
    }

    loginSceneFields = {
        topFields : {id: "loginScene"}
        usernameLabelFields : {id: "userlabel", text: "Email", font: fontType(40)}
        passwordLabelFields : {id: "passwordlabel", text: "Password", font: fontType(40)}
        buttonFields : {id: "button", text: "Log In", iconUri: "", focusedIconUri: "", textFont: fontType(40), focusedTextFont: fontType(40), minWidth: 30}
        keyboardFields : {id: "keyboard", visible: false, showTextEditBox: false}
        mainLayoutFields : {id: "mainLayout", layoutDirection: "horiz", itemSpacings: [40]}
        labelLayoutFields : {id: "labelLayout", layoutDirection: "vert", itemSpacings:[50]}
        editBoxLayoutFields : {id: "editBoxLayout", layoutDirection: "vert", itemSpacings:[50]}
        usernameEditBoxFields : {id: "usernameEditBox", clearOnDownKey: false}
        passwordEditBoxFields : {id: "passwordEditBox", clearOnDownKey: false}
    }

    videoSceneFields = {
        topFields : {id: "videoScene"}
        videoFields : {
                id: "video", width:"640", height:"360", translation:[0, 0],
                content: createVideoContent({
                url: "https://roku.s.cpl.delvenetworks.com/media/59021fabe3b645968e382ac726cd6c7b/60b4a471ffb74809beb2f7d5a15b3193/roku_ep_111_segment_1_final-cc_mix_033015-a7ec8a288c4bcec001c118181c668de321108861.m3u8"
                title:"Test Video"
                streamformat: "hls"
            })
        }
    }

    styles                      = {}
    styles.uiResolution         = {width: width, height: height}
    styles.topBarFields         = topBarFields
    styles.topItemFields        = topItemFields
    styles.rowlistFields        = rowListFields
    styles.loginSceneFields     = loginSceneFields
    styles.baseItemFields       = baseItemFields
    styles.detailsSceneFields   = detailsSceneFields
    styles.videoSceneFields     = videoSceneFields

    m.global.styles = styles
end function

sub addGlobalFields()
    m.global.addField("styles", "assocarray", false)
	m.global.addField("logger", "node", false)

    m.global.logger = createObject("roSGNode", "logger")
end sub

function getResolution()
    devInfo = createObject("roDeviceInfo")
    uiResolution = devInfo.getUIResolution()
    m.global.logger.callFunc("consoleLog", {info: uiResolution})
    return uiResolution
end function

function setupGrid(uiResolution)
    n = 4
    width  = uiResolution.width
    height = uiResolution.height

    cols = n
    rows = n

    gridw = width  * 0.8
    gridh = height * 0.8

    cellw = gridw / cols
    cellh = gridh / rows

    gapx  = gridw * 0.01
    gapy  = gridh * 0.01

    itemw = cellw - gapx
    itemh = cellh - gapy

    ix = (width  - gridw) * 0.5
    iy = (height - gridh) * 0.75

    dims = {
        n : n,
        gridw : gridw, gridh : gridh,
        cellw : cellw, cellh : cellh,
        gapx  : gapx,  gapy  : gapy,
        itemw : itemw, itemh : itemh,
        ix : ix, iy: iy
    }
    print dims

    rowListFields = {
        id                      : "rowList"
        itemComponentName       : "baseItem"
        drawFocusFeedback       : true
        numRows                 : rows
        translation             : [ix, iy]
        itemSize                : [gridw, cellh]
        rowItemSize             : [itemw, itemh]
        rowItemSpacing          : [gapx, 0]
        itemSpacing             : [0, gapy]
    }

    itemFields = {
        posterFields : {
            id: "poster"
            width: itemw
            height: itemh
        }

        labelFields : {
            horizAlign:"right"
            color: "#FFFFFF"
            width: itemw
            font: fontType(20)
            ellipsizeOnBoundary: true,
        }

        metadataLayoutFields : {
            id:"metadataLayout"
            translation:[0, itemh * 0.75]
            layoutDirection: "vert"
        }
    }
    return { rowlistFields: rowListFields, itemFields: itemFields }
end function

function setupMenu(uiResolution)
    genres = [
        {"genreId": 0,  "genre": "All"},
        {"genreId": 18, "genre": "Drama"},
        {"genreId": 16, "genre": "Animation"},
        {"genreId": 35, "genre": "Comedy"},
        {"genreId": 80, "genre": "Crime"},
        {"genreId": 99, "genre": "Documentary"},
    ]

    width  = uiResolution.width
    height = uiResolution.height

    cols = genres.count()
    rows = 1

    gridw = width  * 0.8
    gridh = height * 0.075

    cellw = gridw / cols
    cellh = gridh / rows

    gapx  = gridw * 0.01
    gapy  = gridh * 0.01

    itemw = cellw - gapx
    itemh = cellh - gapy

    ix = (width  - gridw) * 0.5
    iy = (height - gridh) * 0.05

    rowlistFields = {
        id                  : "topBar"
        itemComponentName   : "topItem"
        drawFocusFeedback   : false
        translation         : [ix, iy]
        itemSize            : [gridw, cellh]
        rowItemSize         : [itemw, itemh]
        rowItemSpacing      : [gapx, 0]
        content             : createContent(genres)
    }

    itemFields = {
        layoutFields : {
            itemSpacings    : [0],
            layoutDirection : "vert"
        }
        labelFields : {
            width       : itemw,
            height      : itemh * 0.85,
            horizAlign  : "center",
            vertAlign   : "center",
            color       : "0x7C7C7C"
            font        : fontType(30),
        }
        rectangleFields : {
            width   : itemw,
            height  : itemh * 0.15,
            color   : "#0261fa",
            visible : false,
        }
    }

    return { rowlistFields: rowlistFields, itemFields:  itemFields }
end function

function getItemSizeDimensions(numItems, uiResolution, itemSpacingRatio)
    ratio = uiResolution.width/uiResolution.height
    itemSize = {}
    itemSize.width = uiResolution.width/numItems
    itemSize.height = (uiResolution.width/numItems)*ratio
    itemSize.itemSpacing = ratio*itemSpacingRatio
    return itemSize
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

function createVideoContent(items)
    content = createObject("roSGNode", "contentNode")
    content.update({fields: items}, true)
    return content
end function

function fontType(fontSize)
    font = createObject("roSGNode", "font")
    font.uri = "font:BoldSystemFontFile"
    font.size = (fontSize/1000)*m.uiResolution.height
    return font
end function