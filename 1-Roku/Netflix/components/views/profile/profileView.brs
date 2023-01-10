sub init()
    ?"init() profileView"
    m.app = App()
    m.rowListConfig = ProfileConfig()
    m.rowList = m.top.findNode("rowList")
    m.rowList.setFocus(true)
    m.rowList.observeField("itemSelected", "onProfileSelected")
    m.top.observeField("visible", "OnVisibleChange")
    m.top.observeField("focusedChild", "OnFocusedChild")
    
    createTask()
end sub 

sub createTask()
    m.contentTask = CreateObject("roSGNode", "CreateContentTask")
    m.contentTask.observeField("output", "onContentReady")
    m.contentTask.control = "RUN" 
end sub 

sub onContentReady()
    m.rowList.content = m.contentTask.output
    m.contentTask.control = "STOP"
    m.contentTask.unobserveField("output")
    m.contentTask = invalid
    m.rowList.update(m.rowListConfig)
end sub

function ProfileConfig()
    labelSizeY = 10
    offsetFocused = 40
    rowItemSize = m.app.design.profile.item.rowItemSize
    editIconSize = m.app.design.profile.item.editIconSize
    itemSpace = 70
    itemSpacing = count(m.app.design.profile.item.space)
    itemSizeX = (rowItemSize * 5) + (itemSpace * 4) + 40
    itemSizeY = rowItemSize + offsetFocused + editIconSize + itemSpacing + labelSizeY
    xAxis =  ((m.app.design.resolution.width - itemSizeX) / 2)

    return {
        itemSize: [ itemSizeX, itemSizeY],
        rowItemSpacing: [ [ itemSpace, 0 ] ],
        rowItemSize: [[ rowItemSize, itemSizeY]],
        translation: [xAxis, 500] 
    }
end function

sub onProfileSelected()
              
    itemContent = getItemContent()
    backgroundActive= itemContent.backgroundActive
     
    if backgroundActive = false then
        displayEditProfileView(itemContent)
    else
        displayHomeView()  
    endif

end sub


sub displayHomeView()
   m.top.getParent().callFunc("displayHome")
end sub

sub displayEditProfileView(data)
    m.top.getParent().callFunc("displayEditProfile",data)
    m.rowList.setFocus(false)
end sub

function getItemContent()
    rowItemFocused = m.rowList.rowItemFocused '[fila, índice de la fila]
    sectionNumber =  m.rowList.rowItemFocused[0]
    itemIndex = m.rowList.rowItemFocused[1]
    itemContent = m.rowList.content.getChild(0).getChild(itemIndex)
    return itemContent
end function

sub updateEditIcon(key)
    itemContent = getItemContent()
    itemContent.description = key
    ?"[itemContent.description]>>>";itemContent.description 
    if key = "down" then
        itemContent.backgroundActive = false
    elseif key = "up" then
        itemContent.backgroundActive = true 
    endif
end sub


function onKeyEvent(key as String, press as Boolean) as Boolean
    handled = false
        if press then
            if  key = "down" or key = "up" then
                updateEditIcon(key)
                handled = true
            endif
        endif
    return handled
end function


sub onFocusedChild()
    
    ?">>[m.top.isInFocusChain]<<profileView>> ";m.top.isInFocusChain()
       
end Sub

sub onVisibleChange()
    ?">>[m.top.visible]<<profileView>> ";m.top.visible
    
    if m.top.visible then
        m.rowList.setFocus(true)
    endif

end sub