sub init()
    m.layoutGroup = m.top.findNode("layoutGroup")
    setInitialConf()  
end sub

sub setInitialConf()
    text = ["José", "Rodrigo", "Cristian", "Fabian", "Alejo"]
    images = ["pkg:/images/profileIcon.jpg", "pkg:/images/profileIcon_2.jpg", "pkg:/images/profileIcon_3.jpg", "pkg:/images/profileIcon_4.jpg", "pkg:/images/profileIcon_5.jpg"]

    ?"array position [i] -->",text[0], images[3]
    ?"count() function -->",text.count(), images.count()

    for i = 0 to text.count() - 1 
        user        = m.layoutGroup.createChild("customPoster")
        user.text   = text[i]
        user.uri    = images[i]
    end for

    m.focusedNode = 0
    ?"getChild(i) function -->",m.layoutGroup.getChild(3) 
    ?"getChildCount() function -->",m.layoutGroup.getChildCount() 
    m.layoutGroup.getChild(m.focusedNode).setFocus(true)
end sub

function onKeyEvent(key as String, press as Boolean) as Boolean
        handled = false
        if press then
            if key = "right"
                if m.layoutGroup.getChild(m.focusedNode).hasFocus()
                    m.focusedNode++
                    ?key ,m.focusedNode
                    m.layoutGroup.getChild(m.focusedNode).setFocus(true)
                    result = true
                end if
            else if key = "left"
                if m.layoutGroup.getChild(m.focusedNode).hasFocus()
                    m.focusedNode--
                    ?key ,m.focusedNode
                    m.layoutGroup.getChild(m.focusedNode).setFocus(true)
                    result = true
                end if
            end if
        end if                   
    return handled
end function
