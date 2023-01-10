sub init()
    m.rectangle = m.top.findNode("rectangle")
    m.label = m.top.findNode("label")

    m.rectangle.update(m.global.app.itemFields)
    m.label.update(m.global.app.itemFields)
end sub

sub onContentChange(event as object)
    content = event.getData()
    m.label.text = content.title
end sub