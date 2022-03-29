sub init()
    m.posterGrid = m.top.findNode("posterGrid")
    m.titleLabel = m.top.findNode("descriptionLabel")

    m.posterGrid.setFocus(true)
    m.posterGrid.observeField("itemSelected","onItemSelected")
    m.posterGrid.observeField("itemFocused","onItemFocused")
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

sub onItemSelected(event as Object)
    'user presses --> OK
    index = event.getData() 
    rowContent = m.posterGrid.content.getChild(0)
    itemContent = rowContent.getChild(index)
    print (itemContent)
end sub

sub onItemFocused(event as Object)
    'user presses --> left, right, up or down
    index = event.getData()
    rowContent = m.posterGrid.content.getChild(0)
    itemContent = rowContent.getChild(index)
    m.titleLabel.text = itemContent.shortDescriptionLine2
end sub