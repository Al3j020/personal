sub main()
    fibonnaci()
    showHomeScreen()
end sub

sub showHomeScreen()
    screen = createObject("roSGScreen")
    m.port = createObject("roMessagePort")
    screen.setMessagePort(m.port)

    scene = screen.CreateScene("mainScene")
    screen.show()
    print "·"
    print "········ screen.show() => The screen is shown ········"

    scene.observeField("ready", m.port)
    scene.callFunc("onReady")

    while(true)
        msg = wait(0, m.port)
        msgType = type(msg)
        if msgType = "roSGScreenEvent"
            if msg.isScreenClosed() then return
        else if msgType = "roSGNodeEvent"
            print "·"
            print "········ The rootScene is ready: " + scene.ready.toStr() + " ········"
        end if
    end while
end sub

sub fibonnaci()
    f = [1, 1]
    for i = 0 to 17
        f.push(f[i]+ f[i+1])
    end for
    print "."
    print "········ Fibonacci Series ········"
    print formatJson(f)
end sub

