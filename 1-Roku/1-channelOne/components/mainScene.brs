sub init()
    ?"onInit -->"
    m.button = m.top.findNode("button")
    m.rectangleOne = m.top.findNode("rectangleOne")
    m.rectangleTwo = m.top.findNode("rectangleTwo")
    m.miniKeyboard = m.top.findNode("miniKeyboard")
    
    m.button.setFocus(true)
    m.rectangleOne.observeField("focusedChild", "onFocusChain")
    m.rectangleTwo.observeField("focusedChild", "onFocusChain")
end sub

sub onFocusChain(event as object)
    if m.button.hasFocus()
        m.button.text = "buttonFocused"
    else
        m.button.text = "buttonUnfocused"
    end if

    if m.rectangleOne.hasFocus()
        m.rectangleOne.color = "0xF91010"
    else
        m.rectangleOne.color = "0xFFFFFF"
    end if

    if m.rectangleTwo.hasFocus()
        m.rectangleTwo.color = "0x43FF07"
        m.rectangleTwo.width = 100
        m.rectangleTwo.height = 100
    else
        m.rectangleTwo.color = "0xFFFFFF"
        m.rectangleTwo.width = 300
        m.rectangleTwo.height = 300
    end if
end sub

function onKeyEvent(key as string, press as boolean) as boolean
   result = false
    ?"onKeyEvent -->", key, press
    if press
        if key = "down"
            if m.button.isInFocusChain()
                m.rectangleOne.setFocus(true)
            end if
        else if key = "right"
            if m.rectangleOne.hasFocus()
                m.rectangleTwo.setFocus(true)
            elseif m.rectangleTwo.hasFocus()
                m.miniKeyboard.setFocus(true)
            end if
        else if key = "left"
            if m.miniKeyboard.isInFocusChain()
                m.rectangleTwo.setFocus(true)
            else if m.rectangleTwo.hasFocus()
                m.rectangleOne.setFocus(true)
            end if
        else if key = "up"
            if m.rectangleOne.hasFocus()
                m.button.setFocus(true)
            end if
        end if
    end if 

    return result
end function


