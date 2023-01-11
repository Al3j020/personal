sub init()
    print "MainScene - Init()"
    m.group = m.top.findNode("group")
    setGrid()
end sub

sub setGrid()
    r = getUIResolution()

    gridw = 1080 * 0.6
    gridh = 1080 * 0.6

    ix = (r.width  - gridw) * 0.5
    iy = (r.height - gridh) * 0.95

    rows = 4
    cols = 4

    numItems = rows * cols
   
    cellw = gridw / rows
    cellh = gridh / cols
    
    gapx = cellw * 0.1
    gapy = cellw * 0.1

    itemw = cellw - gapx
    itemh = cellh - gapy

    numPad = "123+456-789*C0=/".split("")

    ' m.inputLabel = createObject("roSGNode", "customItem")
    ' m.inputLabel.content = {
    '     width: gridw
    '     height: gridh * 0.2
    '     rectColor: "#FFFFFF"
    '     textColor: "#000000"
    '     fontSize: gridh * 0.1
    ' }
    ' m.inputLabel.text = "INPUT"
    ' m.inputLabel.translation = [ix, gridh * 0.1]

    ' m.outputLabel = createObject("roSGNode", "customItem")
    ' m.outputLabel.content = {
    '     width: gridw
    '     height: gridh * 0.2
    '     text: "OUTPUT"
    '     rectColor: "#FFFFFF"
    '     textColor: "#000000"
    '     fontSize: gridh * 0.1
    ' }
    ' m.outputLabel.text = "Output"
    ' m.outputLabel.translation = [ix, gridh * 0.35]

    ' m.top.appendChild(m.inputLabel)
    ' m.top.appendChild(m.outputLabel)

    for i = 0 to numItems - 1
        row = fix(i / rows)
        col = i mod cols

        x = ix + col * (itemw + gapx)
        y = iy + row * (itemh + gapy)
    
        item = createObject("roSGNode", "customItem")
        item.translation = [x, y]
        item.content = {
            width: itemw
            height: itemh
            text: numPad[i]
            rectColor: "#FFFFFF"
            textColor: "#000000"
            fontSize: cellh * 0.4
        }
        m.group.appendChild(item)
    end for

    m.top.numRows = rows
    m.top.numCols = cols

    m.top.index = m.top.numCols * m.top.rowIndex + m.top.colIndex
end sub

sub onItemIndexSet()
    m.top.index = m.top.numCols * m.top.rowIndex + m.top.colIndex
end sub

sub onItemFocused()
    item = m.group.getChild(m.top.index)
    item.setFocus(true)
    ' m.inputLabel.text = item.text
    print "itemFocused: ", m.top.index, item.text
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handled = false
    if press
        if key = "left"
            if m.top.colIndex > 0 then m.top.colIndex--
            handled = true
        else if key = "right"
            if m.top.colIndex < m.top.numCols - 1 then m.top.colIndex++
            handled = true
        else if key = "up"
            if m.top.rowIndex > 0 then m.top.rowIndex--
            handled = true
        else if key = "down"
            if m.top.rowIndex < m.top.numRows - 1 then m.top.rowIndex++
            handled = true
        end if
    end if
    return handled
end function


function getBoundingRect(node as object) as object
    return node.boundingRect()
end function

