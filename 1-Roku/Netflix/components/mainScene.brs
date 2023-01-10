sub init()
    
    m.profileView = m.top.findNode("profileView")
    m.homeView = m.top.findNode("homeView")
    m.editProfileView = m.top.findNode("editProfileView")
    m.homeView.visible = false
    m.editProfileView.visible = false
   
 end sub 

 sub displayProfile()
    m.homeView.visible = false
    m.profileView.visible = true
    m.editProfileView.visible = false
end sub

sub displayHome()
    m.homeView.visible = true
    m.profileView.visible = false
    m.editProfileView.visible = false
end sub

function displayEditProfile(data)
    m.editProfileView.visible =true
    m.editProfileView.data = data
    m.profileView.visible = false
    m.homeView.visible = false
end function







