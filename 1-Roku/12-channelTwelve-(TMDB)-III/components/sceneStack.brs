sub initSceneStack()
    m.sceneStack = []
end sub

sub initScene()
    navToScene({ scene: "baseScene" })
end sub

sub navToScene(event as object)
    if type(event) = "roSGNodeEvent"
        data = event.getData()
    else
        data = event
    end if

    scene = createObject("roSGNode", data.scene)

    if data.content <> invalid
        scene.content = data.content
    end if

    scene.components = m.componentConfig[data.scene]
    showScene(scene)
end sub

sub showScene(node as object)
    m.scenes.appendChild(node)
    node.visible = true
    prev = m.sceneStack.peek()
    if prev <> invalid
        prev.visible = false
    end if
    node.setFocus(true)
    m.sceneStack.push(node)
end sub

sub closeScene()
    last = m.sceneStack.pop()
    m.scenes.removeChild(last)
    prev = m.sceneStack.peek()
    if prev <> invalid
        prev.visible = true
        prev.setFocus(true)
    end if
end sub