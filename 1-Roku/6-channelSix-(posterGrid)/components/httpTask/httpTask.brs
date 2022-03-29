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
    shortFormVideos = data.shortFormVideos

    content = createObject("roSGNode", "contentNode")
    rowContent = content.createChild("contentNode")
    rowContent.contentType = "section"
    rowContent.title = "row 0"

    for i = 0 to shortFormVideos.count() - 1 
        itemContent = rowContent.createChild("contentNode")
        itemContent.hdPosterUrl = shortFormVideos[i].thumbnail
        itemContent.shortDescriptionLine1 = shortFormVideos[i].title
        itemContent.shortDescriptionLine2 = shortFormVideos[i].shortDescription  
    end for
   
    response = {
        data: content
    }

    m.top.response = response 
end sub
