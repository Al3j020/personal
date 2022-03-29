sub init()
    bindComponents()
    setStyles()
    fetchContent()
end sub

sub bindComponents()
    m.top.fetcher = createObject("roSGNode","fetcher")
    m.rowlist = m.top.createChild("rowlist")
    m.counter = -1
    m.rows = []
end sub

sub setStyles()
    m.rowlist.setFields(m.global.styles.rowListFields)
    m.rowlist.setFocus(true)
end sub

sub fetchContent()
    fetchTVShows()
    fetchTrending("tv", "week")
end sub

sub fetchTVShows()
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onTVShowsResponse")
    request = {method: "GET", httpNode: httpNode}
    m.top.fetcher.callFunc("fetchTVShows", request)
    m.counter++
end sub

sub fetchTrending(mediaType, timeWindow)
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onHttpResponse")
    request = {
        method          : "GET",
        httpNode        : httpNode,
        queryParams     : {mediaType: mediaType, timeWindow: timeWindow}
    }
    m.top.fetcher.callFunc("fetchTrending", request)
    m.counter++
end sub

sub fetchRecomendations(showId)
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onHttpResponse")
    request = {method: "GET", httpNode: httpNode, showId: showId}
    m.top.fetcher.callFunc("fetchRecomendations", request)
    m.counter++
end sub

sub fetchProviders(showId)
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onHttpResponse")
    request = {method: "GET", httpNode: httpNode, showId: showId}
    m.top.fetcher.callFunc("fetchProviders", request)
    m.counter++
end sub

sub onTVShowsResponse(event as object)
    response = event.getData()
    m.counter--
    if not response.error
        items = response.data.items
        m.rows.push(items)
        for each item in items
            fetchRecomendations(item.id)
        end for
    end if
end sub

sub onHttpResponse(event as object)
    response = event.getData()
    m.counter--
    if not response.error
        m.rows.push(response.data.items)
        if m.counter = 0
            setContent()
        end if
    end if
end sub

sub setContent()
    content = createObject("roSGNode","contentNode")
    for each row in m.rows
        if row.count() > 0
            rowContent = content.createChild("contentNode")
            for each item in row
                itemContent = rowContent.createChild("contentNode")
                itemContent.update(item, true)
            end for
        end if
    end for
    m.rowlist.content = content
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "back"
            m.rowlist.animateToItem = 0
            handle = true
        end if
    end if
    return handle
end function