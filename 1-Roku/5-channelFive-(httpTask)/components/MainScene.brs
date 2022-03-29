sub init()
    m.posterGrid = m.top.findNode("posterGrid")
    m.posterGrid.observeField("itemFocused","onItemFocused")
    m.posterGrid.setFocus(true)
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
    m.posterGrid.content = response.data
    m.httpTask.control = "STOP"
    m.httpTask = invalid
end sub

sub onItemFocused(event as Object)
    index = event.getData()
    row = m.poster.getChild(index[0])
    item = row.getChild(index[1])
    m.titleLabel = item.title
end sub