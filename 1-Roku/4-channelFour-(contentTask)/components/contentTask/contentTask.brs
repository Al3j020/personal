sub init()
    m.top.functionName = "getContent"
end sub

sub getContent()

    'request
    mockData = m.top.request
    genres = mockData.genres

    'response
    content = createObject("roSGNode", "contentNode")
    for i= 0 to genres.count() - 1
        itemContent = content.createChild("contentNode")
        itemContent.title = genres[i].title
        itemContent.hdListItemIconUrl = genres[i].iconFocused
        itemContent.hdListItemIconSelectedUrl = genres[i].iconUnfocused
    end for
    
    response = {
        data: content
    }
    m.top.response = response 
end sub
