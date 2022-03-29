sub init()
    configApp()
    setBaseScene()
end sub

sub setBaseScene()
    baseScene = createObject("roSGNode","BaseScene")
    m.top.appendChild(baseScene)
end sub