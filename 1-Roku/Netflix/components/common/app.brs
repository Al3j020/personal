function App()
    m.app = m.global.app

    if m.app = invalid then

        m.app = {}

        '##### images
        m.app.design = {}
        m.app.design.images = {}
        m.app.design.images.editIcon = "pkg:/images/edit_icon.png"
        m.app.design.images.editIconActive = "pkg:/images/edit_icon_active.png"
        m.app.design.images.focusGrid = "pkg:/images/focus_grid.9.png"

        '#### ui resolution
        uiResolution = getResolution()
        m.app.design.resolution = {}
        m.app.design.resolution.width = uiResolution.width
        m.app.design.resolution.height = uiResolution.height

        '#### Profile
        m.app.design.profile = {}
        m.app.design.profile.item = {}
        m.app.design.profile.item.space = [20, 50]
        m.app.design.profile.item.rowItemSize = 240
        m.app.design.profile.item.editIconSize = 90

        m.global.addFields({app: m.app})
    end if

    return m.app
end function


function getResolution()
    devInfo = createObject("roDeviceInfo")
    supportedResolutions = devInfo.getSupportedGraphicsResolutions()
    uiResolution = supportedResolutions[supportedResolutions.count()-1]
    return uiResolution
end function