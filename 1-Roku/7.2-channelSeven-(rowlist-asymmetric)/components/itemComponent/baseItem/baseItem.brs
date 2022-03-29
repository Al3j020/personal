sub onItemContent(event as object)
    itemContent = event.getData()
  
    if itemContent.id = "row_0"
        m.smallItem = m.top.createChild("smallItem")
        m.smallItem.itemContent = itemContent

    else if itemContent.id = "row_1"
        m.mediumItem = m.top.createChild("mediumItem")
        m.mediumItem.itemContent = itemContent

    else if itemContent.id = "row_2"
        m.largeItem = m.top.createChild("largeItem")
        m.largeItem.itemContent = itemContent

    end if
end sub