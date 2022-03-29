sub init()
    m.markupList = m.top.findNode("markuplist")
    m.markupList.setFocus(true)
    createTask()
end sub

sub createTask()    
    request = {
        method          : "GET"
        url             : "http://gateway.marvel.com/v1/public/comics?ts=1&apikey=31c3bfa772e000cac3e15637b9d2b913&hash=20ab00b0c0c60160d731f4294c8c28e7&limit=40"
        certificateFile : "common:/certs/ca-bundle.crt"
    }

    m.httpTask = createObject("roSGNode", "httpTask")
    m.httpTask.observeField("response", "onResponseTask")
    m.httpTask.request = request 
    m.httpTask.control = "RUN" 
end sub

sub onResponseTask(event as Object)
    response = event.getData()
    m.markupList.content = response.data
    m.httpTask.control = "STOP"
    m.httpTask = invalid

    m.markupList.observeField("itemSelected","onItemSelected")
    m.markupList.observeField("itemFocused","onItemFocused")
end sub

sub onItemSelected(event as Object)
    itemIndex = event.getData()
end sub

sub onItemFocused(event as Object)
    itemIndex = event.getData()
end sub