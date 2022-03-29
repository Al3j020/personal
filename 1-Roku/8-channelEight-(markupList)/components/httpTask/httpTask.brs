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
    comics = data.data.results

    content = createObject("roSGNode", "contentNode")

    for each comic in comics
        itemContent = content.createChild("contentNode")
        itemContent.update({
            title       : comic.title
            description : comic.description
            hdPosterUrl : comic.thumbnail.path + "." + comic.thumbnail.extension
        })
    end for

    response = {
        data: content
    }

    m.top.response = response 
end sub

