sub init()
    m.rowlist = m.top.findNode("rowlist")
    m.animation = m.top.findNode("animation")
    m.vector2D = m.top.findNode("vector2D")
    m.rowlist.setFocus(true)
    createTask()
end sub

sub createTask()    
    request = {
        method          : "GET"
        url             : "https://jonathanbduval.com/roku/feeds/roku-developers-feed-v1.json"
        certificateFile : "common:/certs/ca-bundle.crt"
    }
   
    m.httpTask = createObject("roSGNode", "httpTask")
    m.httpTask.observeField("response", "onResponseTask")
    m.httpTask.request = request 
    m.httpTask.control = "RUN" 
end sub

sub onResponseTask(event as Object)
    response = event.getData()
    m.rowlist.content = response.data
    m.httpTask.control = "STOP"
    m.httpTask = invalid

    m.rowlist.observeField("rowItemSelected","onItemSelected")
    m.rowlist.observeField("rowItemFocused","onItemFocused")
end sub

sub onItemSelected(event as Object)
    rowItemIndex = event.getData()
    rowContent = m.rowlist.content.getChild(rowItemIndex[0])
    ' itemContent = rowContent.getChild(rowItemIndex[1])
    ' if rowContent.live = false
    '     m.rowlist.drawFocusFeedback = false
    '     rowContent.live = true
    ' else
    '     m.rowlist.drawFocusFeedback = true
    '     rowContent.live = false
    ' end if
end sub

sub onItemFocused(event as Object)
    rowItemIndex = event.getData()
    rowContent = m.rowlist.content.getChild(rowItemIndex[0])
    itemContent = rowContent.getChild(rowItemIndex[1])

    if rowContent.live = false
        rowContent.live = true
    else
        rowContent.live = false
    end if
    rowContent.rowIndex = rowItemIndex[0]
end sub