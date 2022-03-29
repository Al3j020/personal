sub init()
    ' setStyles()
    ' setObservers()
end sub

sub addComponents()
    for each key in m.top.components
        node = createObject("roSGNode", m.top.components[key])
        node.id = key
        m[key] = node
    end for

    m.vertLeftLayout.insertChildren([m.userLabel, m.passwordLabel], 0)
    m.vertRightLayout.insertChildren([m.usernameEditBox, m.passwordEditBox, m.button], 0)
    m.horizMainLayout.insertChildren([m.vertLeftLayout, m.vertRightLayout], 0)

    m.top.insertChildren([m.horizMainLayout, m.keyboard], 0)

    m.textEditBoxId = ""
    m.textInput = ""
    m.userInfo = {}
    m.focusInfo = {}
    setStyles()
    setObservers()
end sub

sub setStyles()
    styles = m.global.styles
    m.top.setFields(styles.loginSceneFields.topFields)
    m.userLabel.setFields(styles.loginSceneFields.usernameLabelFields)
    m.passwordLabel.setFields(styles.loginSceneFields.passwordLabelFields)
    m.usernameEditBox.setFields(styles.loginSceneFields.usernameEditBoxFields)
    m.passwordEditBox.setFields(styles.loginSceneFields.passwordEditBoxFields)
    m.button.setFields(styles.loginSceneFields.buttonFields)
    m.keyboard.setFields(styles.loginSceneFields.keyboardFields)
    m.horizMainLayout.setFields(styles.loginSceneFields.mainLayoutFields)
    m.vertLeftLayout.setFields(styles.loginSceneFields.labelLayoutFields)
    m.vertRightLayout.setFields(styles.loginSceneFields.editBoxLayoutFields)

    uiResolution = styles.uiResolution
    mainLayoutBoundingRect = m.horizMainLayout.boundingRect()
    keyboardBoundingRect = m.keyboard.boundingRect()
    m.horizMainLayout.translation = [(uiResolution.width -  mainLayoutBoundingRect.width) * 0.5, uiResolution.height * 0.1]
    m.keyboard.translation = [(uiResolution.width -  keyboardBoundingRect.width) * 0.5, uiResolution.height * 0.5]
end sub

sub setObservers()
    m.top.observeField("focusedChild","onTopFocusChange")
    m.keyboard.observeField("focusedChild","onKeyboardFocusChange")
    m.usernameEditBox.observeField("focusedChild","onUsernameFocusChange")
    m.passwordEditBox.observeField("focusedChild","onPasswordFocusChange")
    m.button.observeField("buttonSelected","onButtonSelected")
    m.keyboard.textEditBox.observeField("text","onTextInput")
end sub

sub onTopFocusChange(event as object)
    ' node = event.getData()
    ' if type(node) <> "roInvalid" then print node.id, node.subType()

    ' print "·"
    ' print "m.top             : ", "hasFocus(): ", m.top.hasFocus(),              "isInFocusChain(): ", m.top.isInFocusChain()
    ' print "m.keyboard        : ", "hasFocus(): ", m.keyboard.hasFocus(),         "isInFocusChain(): ", m.keyboard.isInFocusChain()
    ' print "m.usernameEditBox : ", "hasFocus(): ", m.usernameEditBox.hasFocus(),  "isInFocusChain(): ", m.usernameEditBox.isInFocusChain()
    ' print "m.passwordEditBox : ", "hasFocus(): ", m.passwordEditBox.hasFocus(),  "isInFocusChain(): ", m.passwordEditBox.isInFocusChain()
    ' print "m.button          : ", "hasFocus(): ", m.button.hasFocus(),           "isInFocusChain(): ", m.button.isInFocusChain()

    if m.top.hasFocus()
        m.usernameEditBox.setFocus(true)
    end if
end sub

sub onKeyboardFocusChange(event as object)
    if m.keyboard.isInFocusChain()
        m.keyboard.visible = true
    else
        m.keyboard.visible = false
    end if
end sub

sub onUsernameFocusChange(event as object)
    if m.usernameEditBox.hasFocus()
        m.usernameEditBox.active = true
    else
        m.usernameEditBox.active = false
    end if
end sub

sub onPasswordFocusChange(event as object)
    if m.passwordEditBox.hasFocus()
        m.passwordEditBox.active = true
    else
        m.passwordEditBox.active = false
    end if
end sub

sub onButtonSelected(event as object)
    rootScene = m.top.getScene()
    rootScene.router = {scene : "baseScene"}
end sub

sub onTextInput(event as object)
    if type(event) = "roSGNodeEvent"
        m.textInput = event.getData()
    else
        m.textEditBoxId = event
    end if

    if m.textInput <> ""
        if m.textEditBoxId = "usernameEditBox"
            m.usernameEditBox.text = m.textInput
        else
            m.passwordEditBox.text = m.textInput
        end if
    end if
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "OK"
            if m.usernameEditBox.hasFocus() or m.passwordEditBox.hasFocus()
                if m.usernameEditBox.hasFocus()
                    onTextInput(m.usernameEditBox.id)
                else
                    onTextInput(m.passwordEditBox.id)
                end if
                m.keyboard.setFocus(true)
                handle = true
            end if
        else if key = "down"
            if m.usernameEditBox.hasFocus()
                m.passwordEditBox.setFocus(true)
                handle = true
            else if m.passwordEditBox.hasFocus()
                m.button.setFocus(true)
                handle = true
            end if
        else if key = "up"
            if m.button.hasFocus()
                m.passwordEditBox.setFocus(true)
                handle = true
            else if m.passwordEditBox.hasFocus()
                m.usernameEditBox.setFocus(true)
                handle = true
            else if m.keyboard.isInFocusChain() and m.textEditBoxId = "usernameEditBox"
                saveData("username")
                m.usernameEditBox.setFocus(true)
            else if m.keyboard.isInFocusChain() and m.textEditBoxId = "passwordEditBox"
                saveData("password")
                m.passwordEditBox.setFocus(true)
                handle = true
            end if
        end if
    end if
    return handle
end function

sub saveData(key)
    m.userInfo[key] = m.keyboard.textEditBox.text
    m.keyboard.textEditBox.text = ""
    print formatJson(m.userInfo)
end sub