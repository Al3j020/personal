sub init()
    ?"init() editProfileView"
   'RightSide
    m.editLabel= m.top.findNode("editLabel")
    m.selectLabel= m.top.findNode("selectLabel")
    m.editMenu= m.top.findNode("editMenu")
    m.ratingLabel= m.top.findNode("ratingLabel")
    m.button= m.top.findNode("button")
   'LeftSide
    m.poster = m.top.findNode("poster")
    m.profileLabel= m.top.findNode("profileLabel")
    
    m.top.observeField("focusedChild", "OnFocusedChildEditView")
    m.top.observeField("visible", "OnVisibleChange")
   
    
    'Update profileData
    m.top.observeField("data", "updateProfile")   
end sub 

sub updateProfile()
    data = m.top.data
    m.poster.uri=data.HDPOSTERURL
    m.profileLabel.text = data.title
end sub


sub onFocusedChildEditView()
      
    ?">>[m.top.isInFocusChain]<<EditView>> ";m.top.isInFocusChain()
    '?">>[m.top.hasFocus]<<EditView>> ";m.top.hasFocus()
    '?">>[m.top.setFocus]<<EditView>> ";m.top.setFocus(true)
       
end sub

sub onVisibleChange()
    ?">>[m.top.visible]<<EditView>> ";m.top.visible
    if m.top.visible then
        m.editMenu.setFocus(true)
    endif
end sub

function onKeyEvent(key as String, press as Boolean) as Boolean
    
    ?"key>>";key;" ";"press>>";press
    handled = false
        
        if press then
           
            if  key = "back" then
                m.top.getParent().callFunc("displayProfile")
                handled=true
            elseif  key = "down"
                m.button.setFocus(true)
                handled=false
            elseif  key = "up"
                m.editMenu.setFocus(true)
                handled=false
            endif
        endif
    
    return handled
end function



