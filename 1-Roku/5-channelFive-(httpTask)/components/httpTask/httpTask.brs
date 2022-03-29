sub init()
    m.top.functionName = "getContent"
end sub

sub getContent()
    'request
    request = m.top.request
    urlTransfer = createObject("roUrlTransfer")
    urlTransfer.setUrl(request.url)         'set url
    urlTransfer.setCertificatesFile(request.certificateFile)
    urlTransfer.InitClientCertificates()
    stringBody = urlTransfer.getToString() 'method : GET
   
    'response
    data = parseJson(stringBody)
    print(data)

    shortFormVideos = data.shortFormVideos
    movies = []
    series = []

    content = createObject("roSGNode", "contentNode")
    rowContent = content.createChild("contentNode")
    rowContent.contentType = "SECTION"
    rowContent.title = "List 1"

    for i = 0 to shortFormVideos.count() - 1
        itemContent = rowContent.createChild("contentNode") 
        itemContent.shortDescriptionLine1 = shortFormVideos[i].title
        itemContent.hdPosterUrl = shortFormVideos[i].thumbnail
    end for

    response = {
        data: content
    }
    m.top.response = response 
end sub
