sub init()
    bindComponents()
    setStyles()
end sub

sub bindComponents()
    m.label     = createObject("roSGNode", "label")
    m.rectangle = createObject("roSGNode","rectangle")
    m.layout    = createObject("roSGNode", "layoutGroup")

    m.layout.insertChildren([m.label, m.rectangle], 0)
    m.top.appendChild(m.layout)
end sub

sub setStyles()
    styles = m.global.styles
    m.layout.setFields(styles.topItemFields.layoutFields)
    m.label.setFields(styles.topItemFields.labelFields)
    m.rectangle.setFields(styles.topItemFields.rectangleFields)
end sub

sub onItemContent(event as object)
    content = event.getData()
    m.label.text = content.genre
end sub

sub onFocusPercent()
    if m.top.rowListHasFocus
        if m.top.focusPercent > 0.5
            m.label.color = "0xFFFFFF"
            m.rectangle.visible = true
        else
            m.label.color = "0x7C7C7C"
            m.rectangle.visible = false
        end if
    else
        m.rectangle.visible = false
    end if
end sub

sub onRowListHasFocus()
    onFocusPercent()
end sub