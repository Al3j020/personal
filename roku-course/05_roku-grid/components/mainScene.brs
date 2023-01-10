sub init()
    print "MainScene - Init()"
    m.rowlist = m.top.findNode("rowlist")
    setGrid()
end sub

sub setGrid()
    m.app = app()
    m.rowlist.update(m.app.rowListFields, true)
    m.rowlist.setFocus(true)
end sub
