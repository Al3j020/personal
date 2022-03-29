sub init()
    m.rowlist = m.top.findNode("rowlist")
    setRowlistConfig()
end sub

sub setRowlistConfig()
    buttonArray = ["Home", "Live", "Sports", "News", "Kids", "Shows"]
    content = createObject("roSGNode", "contentNode")
    rowContent = content.createChild("contentNode")

    for i = 0 to buttonArray.count() - 1
        itemContent = rowContent.createChild("contentNode")
        itemContent.title = buttonArray[i]
    end for

    m.rowlist.content = content
    m.rowlist.setFocus(true)
end sub