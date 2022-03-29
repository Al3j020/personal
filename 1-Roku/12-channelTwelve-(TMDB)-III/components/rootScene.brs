sub init()
    initComponents()
    initSceneStack()
    initConfig()
    initScene()
end sub

sub initComponents()
    m.scenes = m.top.findNode("scenes")
    m.componentConfig = componentConfig()
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "back"
            closeScene()
            handle = true
        end if
    end if
    return handle
end function