sub initScreenStack()
    m.screenStack = []
end sub

sub showScreen(node as object)
    m.top.appendChild(node)
    node.visible = true
    prev = m.screenStack.peek()
    if prev <> invalid
        prev.visible = false
    end if
    node.setFocus(true)
    m.screenStack.push(node)
end sub

sub closeScreen()
    last = m.screenStack.pop()
    m.top.removeChild(last)
    prev = m.screenStack.peek()
    if prev <> invalid
        prev.visible = true
        prev.setFocus(true)
    end if
end sub