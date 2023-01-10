function app()
    m.app = m.global.app

    if m.app = invalid then
        m.app = {}

        ' content
        buttons = ["Action", "Animation", "Sci-Fi & Fantasy", "Comedy", "Western"]
        content = createObject("roSGNode", "contentNode")
        rowContent = content.createChild("contentNode")

        for each button in buttons
            itemContent = rowContent.createChild("contentNode")
            itemContent.title = button
        end for

        ' UI fields
        r = getUIResolution()

        gridw = r.width  * 0.9
        gridh = r.height * 0.25

        ix = (r.width  - gridw) * 0.5
        iy = (r.height - gridh) * 0.5

        rows = 1
        cols = buttons.count()

        cellw = gridw / cols
        cellh = gridh / rows

        gapx = cellw * 0.1
        gapy = cellw * 0.1

        itemw = cellw - gapx
        itemh = cellh - gapy

        rowListFields = {
            id                  : "rowList"
            itemComponentName   : "customItem"
            content             : content
            drawFocusFeedback   : true
            numRows             : rows
            translation         : [ix, iy]
            itemSize            : [gridw, cellh]
            rowItemSize         : [itemw, itemh]
            rowItemSpacing      : [gapx, 0]
            itemSpacing         : [0, gapy]  
        }

        itemFields = {
            id      : "customItem"
            width   : itemw
            height  : itemh
        }

        m.app.rowlistFields = rowListFields
        m.app.itemFields = itemFields

        m.global.addFields({ app: m.app })
    end if
    
    return m.app
end function