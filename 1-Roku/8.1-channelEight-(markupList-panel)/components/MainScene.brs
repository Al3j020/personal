sub init()
    m.panelList = m.top.findNode("panellist")
    m.panelList.setFocus(true)
    createPanel()
end sub

sub createPanel()
    content = createObject("roSGNode", "contentNode")
    buttonArray = ["Home", "Series", "Movies", "Hub"]

    for each button in buttonArray
        itemContent = content.createChild("contentNode")
        itemContent.title = button
    end for

    m.panelList.content = content
end sub

sub onItemSelected(event as Object)
    itemIndex = event.getData()
end sub

sub onItemFocused(event as Object)
    itemIndex = event.getData()
end sub