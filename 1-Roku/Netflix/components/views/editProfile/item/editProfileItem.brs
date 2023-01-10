sub init()
    ?"init() editProfileItem"
    m.layoutGroup = m.top.findNode("layoutGroup")
    m.itemPoster = m.top.findNode("itemPoster")
    m.title = m.top.findNode("title")
end sub

sub onItemContentChanged()
    m.item = m.top.itemContent
    m.itemPoster.uri = m.item.HDPOSTERURL
    m.title.text = m.item.title
    
end sub

sub onUserHasFocus()
?"onUserHasFocus() ";m.top.focusPercent 
end sub
