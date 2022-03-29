sub init()
    initScreenStack()
    configApp()
    setBaseScene()
    m.global.observeField("newSceneCTA", "onNewSceneCTA")
end sub

sub setBaseScene()
    baseScene = createObject("roSGNode","BaseScene")
    showScreen(baseScene)
end sub

sub onNewSceneCTA(event as object)
    newSceneCTA = event.getData()
    newScene = createObject("roSGNode", newSceneCTA.node)
    newScene.content = newSceneCTA.content
    showScreen(newScene)
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "back"
            closeScreen()
            handle = true
        end if
    end if
    return handle
end function