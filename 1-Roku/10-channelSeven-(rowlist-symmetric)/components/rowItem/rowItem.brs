sub init()
    m.rectangle = m.top.findNode("rectangle")
    m.button = m.top.findNode("button")
end sub

sub onContentChange(event as object)
    rowContent = event.getData()
    ?rowContent.rowIndex, rowContent.live
    if rowContent.live
        m.button.text = "[focus]"
        m.button.setFocus(true)
    else
        m.button.text = "[unfocus]"
        m.button.setFocus(false)
    end if
end sub