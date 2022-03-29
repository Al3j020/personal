sub init()
    m.port = createObject("roMessagePort")
    m.top.observeField("request", m.port)
    m.httpTransfer = {}
    m.top.functionName = "initHttpTask"
end sub

sub initHttpTask()
    while true
        msg = wait(0, m.port)
        if type(msg) = "roSGNodeEvent"
            handleRequest(msg)
        else if type(msg) = "roUrlEvent"
            handleResponse(msg)
        end if
    end while
end sub

sub handleRequest(event as object)
    request = event.getData()
    urlTransfer = createObject("roUrlTransfer")
    urlTransfer.setCertificatesFile("common:/certs/ca-bundle.crt")
    urlTransfer.initClientCertificates()
    urlTransfer.setUrl(request.url)
    urlTransfer.setMessagePort(m.port)

    if request.method = "GET"
        urlTransfer.asyncGetToString()
    end if

    requestId = urlTransfer.getIdentity().toStr()
    m.httpTransfer[requestId] = {urlTransfer: urlTransfer, request: request}
end sub

sub handleResponse(event as object)
    code        = event.getResponseCode()
    requestId   = event.getSourceIdentity().toStr()
    request     = m.httpTransfer[requestId].request

    if code >= 200 and code < 300
        body = event.getString()
        data = parseJson(body)
        model = createModel({data:data, kind:request.model})
        sendResponse(request, model.data, code, false)
        logMessage(code, event, request)
    else
        logMessage(code, event, request)
    end if
end sub

function createModel(modelInfo as object)
    model = createObject("roSGNode", modelInfo.kind)
    model.callFunc("parseData", modelInfo.data)
    return model
end function

sub sendResponse(request, data, code, error)
    if request.httpNode <> invalid
        httpNode = request.httpNode
        httpNode.response = {code: code, data: data, error: error}
    end if
end sub

sub logMessage(code, event, request)
    info = substitute("status: {0} {1} url: {2}", code.toStr(), event.getFailureReason(), request.url)
    m.global.logger.callFunc("consoleLog", {info: info})
end sub