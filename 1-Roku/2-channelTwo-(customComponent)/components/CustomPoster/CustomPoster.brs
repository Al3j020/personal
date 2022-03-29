sub init()
    m.rectangle = m.top.findNode("rectangle")
    m.top.observeField("focusedChild", "onFocusChange")
end sub

sub onFocusChange(event as Object)
    if m.top.hasFocus()
        m.rectangle.visible = true 
    else
        m.rectangle.visible = false
    end if
end sub