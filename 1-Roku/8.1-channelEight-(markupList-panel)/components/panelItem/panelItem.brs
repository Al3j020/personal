sub init()
    m.label = m.top.findNode("label")
    m.rectangle = m.top.findNode("rectangle")
end sub

sub onItemContent(event as object)
    itemContent = event.getData()
    m.label.text = itemContent.title
end sub


sub onFocusPercent(event as object)
    focusPercent = event.getData()
    if focusPercent > 0.5
        m.rectangle.visible = true
    else
        m.rectangle.visible = false
    end if
end sub