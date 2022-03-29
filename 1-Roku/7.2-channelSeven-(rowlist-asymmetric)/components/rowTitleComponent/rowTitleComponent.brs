sub init()
    m.label = m.top.findNode("label")

end sub
sub onContentChange(event as Object)
    content = event.getData()
    ?"onRowTitleComponent -->",content.title
    m.label.text = content.title
end sub