sub init()
    print "MainScene - Init()"
    m.layoutGroup = m.top.findNode("layoutGroup")
    m.fishProfile = m.top.findNode("fishProfile")
    m.birdProfile = m.top.findNode("birdProfile")
    
    setConfig()

    m.fishProfile.observeField("focusedChild", "onFocusChange")
    m.birdProfile.observeField("focusedChild", "onFocusChange")

    m.fishProfile.setFocus(true)
end sub

sub setConfig()
    r = getResolution()
    b = m.layoutGroup.boundingRect()
    x = (r.width  - b.width ) * 0.5
    y = (r.height - b.height) * 0.5

    m.layoutGroup.translation = [x, y]
end sub

sub onFocusChange(event as object)
    node =  event.getRoSGNode()
    print "onFocusChange", node.id, m.fishProfile.hasFocus(), m.birdProfile.hasFocus()
    if m.fishProfile.hasFocus()
        m.fishProfile.findNode("fishRect").color = "#FF00FF"
        m.fishProfile.opacity = 1.0
    else
        m.fishProfile.findNode("fishRect").color = "#FFFFFF"
        m.fishProfile.opacity = 0.25
    end if

    if m.birdProfile.hasFocus()
        m.birdProfile.findNode("birdRect").color = "#FF00FF"
        m.birdProfile.opacity = 1.0
    else
        m.birdProfile.findNode("birdRect").color = "#FFFFFF"
        m.birdProfile.opacity = 0.25
    end if
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "right"
            if m.fishProfile.hasFocus() then m.birdProfile.setFocus(true)
        else key = "left"
            if m.birdProfile.hasFocus() then m.fishProfile.setFocus(true)
        end if
    end if
    return handle
end function

function getResolution() as object
    di = createObject("roDeviceInfo")
    return di.getUIResolution()
end function
