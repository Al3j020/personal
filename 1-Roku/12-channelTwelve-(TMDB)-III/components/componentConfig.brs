function ComponentConfig() as Object
    if(m.componentConfig = invalid) then
        m.componentConfig = {
            loginScene: {
                keyboard        : "keyboard",
                userLabel       : "label",
                passwordLabel   : "label",
                usernameEditBox : "textEditBox",
                passwordEditBox : "textEditBox",
                button          : "button",
                vertLeftLayout  : "layoutGroup",
                vertRightLayout : "layoutGroup",
                horizMainLayout : "layoutGroup",
            },
            baseScene: {
                topBar  : "rowlist",
                rowlist : "rowlist"
            },
            detailsScene: {
                titleLabel          : "label",
                descriptionLabel    : "label",
                metadataLabel       : "label",
                poster              : "poster",
                buttonGroup         : "buttonGroup",
                baseLayout          : "layoutGroup",
                metadataLayout      : "layoutGroup",
                baseLayout          : "layoutGroup",
            },
            videoScene: {
                video       : "video",
                buttonGroup : "buttonGroup",
            },
            baseItem: {
                poster          : "poster",
                rectangle       : "rectangle",
                titleLabel      : "label",
                releaseLabel    : "label",
                scoreLabel      : "label",
                baseLayout      : "layoutGroup",
                metadataLayout  : "layoutGroup",
            },
        }
    end if
    return m.componentConfig
end function