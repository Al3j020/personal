sub init()
    m.labelList = m.top.findNode("labelList")
    m.content = m.top.findNode("content")
    m.labelList.content =  m.content
    m.labelList.setFocus(true)
end sub