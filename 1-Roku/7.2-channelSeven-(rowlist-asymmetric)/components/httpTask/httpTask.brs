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

    content = asymmetricModel(shortFormVideos)
   
    response = {
        data: content
    }

    m.top.response = response 
end sub

function asymmetricModel(data as Object)
    content = createObject("roSGNode", "contentNode")
    
    for i = 0 to 3
        rowContent = content.createChild("contentNode")
        rowContent.title = "row " + i.toStr()
    
        for j = 0 to data.count() - 1 
            itemContent = rowContent.createChild("contentNode")
            itemContent.id = "row_" + i.toStr()
            itemContent.hdPosterUrl = data[j].thumbnail
        end for
    end for

    return content
end function