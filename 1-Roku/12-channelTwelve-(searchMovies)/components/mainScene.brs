sub init()
    configApp()
    setStyles()
    setBaseScene()
end sub

sub setBaseScene()
    baseScene = createObject("roSGNode","BaseScene")
    baseScene.setFocus(true)
    m.top.appendChild(baseScene)
end sub

sub setStyles()
    m.top.setFields(m.global.styles.mainScene)
end sub