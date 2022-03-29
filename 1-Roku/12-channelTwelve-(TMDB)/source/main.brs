sub main()
    screen = createObject("roSGScreen")
    m.port = createObject("roMessagePort")
    screen.setMessagePort(m.port)
    m.global = screen.getGlobalNode()
    scene = screen.createScene("MainScene")
    screen.show()
    newLogger = createObject("roSGNode", "logger")
    m.global.addFields({newLogger: newLogger})
    ?m.global
    while(true)
        msg = wait(0, m.port)
        msgType = type(msg)
        if msgType = "roSGScreenEvent"
            if msg.isScreenClosed() then return
        end if
    end while
end sub

