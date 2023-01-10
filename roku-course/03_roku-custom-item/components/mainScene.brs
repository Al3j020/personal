sub init()
    print "MainScene - Init()"
    m.layoutGroup = m.top.findNode("layoutGroup")
    setConfig()
    m.itemIndex = 0
    m.layoutGroup.getChild(m.itemIndex).setFocus(true)
end sub

sub setConfig()
    r = getResolution()
    b = m.layoutGroup.boundingRect()
    x = (r.width  - b.width ) * 0.5
    y = (r.height - b.height) * 0.5

    m.layoutGroup.translation = [x, y]
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "right"
            if m.itemIndex < m.layoutGroup.getChildCount() - 1
                m.itemIndex++
                m.layoutGroup.getChild(m.itemIndex).setFocus(true)
                print "itemIndex++ ", m.itemIndex 
            end if
        else key = "left"
            if m.itemIndex > 0 
                m.itemIndex--
                m.layoutGroup.getChild(m.itemIndex).setFocus(true)
                print "itemIndex-- ", m.itemIndex
            end if
        end if
    end if
    return handle
end function

function getResolution() as object
    di = createObject("roDeviceInfo")
    return di.getUIResolution()
end function
