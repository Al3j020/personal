sub init()
    m.app = App()
    m.focusedBackground = m.top.findNode("focusedBackground")
    m.layoutGroup = m.top.findNode("layoutGroup")
    m.itemPoster = m.top.findNode("itemPoster")
    m.title = m.top.findNode("title")
    m.editImagePoster = m.top.findNode("editImagePoster")

    applyConfig()
end sub

sub onItemContentChanged()
   
    m.item = m.top.itemContent
    m.title.text = m.item.title
    m.itemPoster.uri = m.item.HDPOSTERURL
    m.editImagePoster.uri = m.app.design.images.editIcon
    
    
    if  m.item.description = "down" then
        m.editImagePoster.uri = m.app.design.images.editIconActive
        m.focusedBackground.visible = false
    elseif  m.item.description = "up"
        m.editImagePoster.uri = m.app.design.images.editIcon
        m.focusedBackground.visible = true
    endif

end sub

sub onUserHasFocus()
'?"onUserHasFocus() ";m.top.focusPercent 
    if m.top.focusPercent > 0.5 then
        m.focusedBackground.visible = true
        m.editImagePoster.visible = true
    else
        m.focusedBackground.visible = false
        m.editImagePoster.visible = false
    end if
end sub


sub applyConfig()
    itemSize = m.app.design.profile.item.rowItemSize
    focusItemSize = itemSize + 40
    editIconSize = m.app.design.profile.item.editIconSize
    itemSpacings = m.app.design.profile.item.space
    translation = [ ((itemSize/2) + 20), 20 ]

    'Focus image
    m.focusedBackground.width = focusItemSize
    m.focusedBackground.height = focusItemSize
    m.focusedBackground.uri = m.app.design.images.focusGrid

    'Layout group
    m.layoutGroup.translation = translation
    m.layoutGroup.itemSpacings = itemSpacings

    'ItemPoster
    m.itemPoster.width = itemSize
    m.itemPoster.height = itemSize

    'EditImagePoster
    m.editImagePoster.width = editIconSize
    m.editImagePoster.height = editIconSize
end sub