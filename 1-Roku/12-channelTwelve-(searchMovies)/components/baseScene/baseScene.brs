sub init()
    bindComponents()
    setStyles()
    setObservers()
    fetchContent()
end sub

sub bindComponents()
    m.top.fetcher = createObject("roSGNode","fetcher")
    m.rowlist = m.top.createChild("rowlist")
    m.keyboard = m.top.createChild("miniKeyboard")
    m.multiStylelabel = m.top.createChild("multiStyleLabel")
    m.counter = -1
    m.rows = []
end sub

sub setStyles()
    m.rowlist.setFields(m.global.styles.baseScene.rowListFields)
    m.keyboard.setFields(m.global.styles.baseScene.keyboardFields)
    m.multiStylelabel.setFields(m.global.styles.baseScene.multiStylelabelFields)
end sub

sub setObservers()
    m.top.observeField("focusedChild","onFocusedChild")
    m.keyboard.observeField("text","onTextChange")
end sub

sub onFocusedChild()
    if m.top.hasFocus()
        m.keyboard.setFocus(true)
    end if
end sub

sub onTextChange(event as object)
    text = event.getData()
    fetchSearch(text)
end sub

sub fetchContent()
    ' fetchTVShows()
    ' fetchTrending("tv", "week")
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

sub fetchSearch(text)
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onHttpResponse")
    request = {method: "GET", httpNode: httpNode, search: text}
    m.top.fetcher.callFunc("fetchSearch", request)
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
    if not response.error
        m.rows.push(response.data.items)
        setContent()
    end if
end sub

sub setContent()
    content = createObject("roSGNode", "contentNode")
    for each row in m.rows
        if row.count() > 0
            i = -1
            j = -1
            rowContent = content.createChild("contentNode")
            for each item in row
                i++
                j++
                if j <= 3
                    itemContent = rowContent.createChild("contentNode")
                    itemContent.update(item, true)
                else if i < row.count() - 1
                    rowContent = content.createChild("contentNode")
                    j = -1
                end if
            end for
        end if
    end for

    m.rowlist.content = content
    m.rows = invalid
    m.rows = []
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "right"
            if m.keyboard.isInFocusChain() then m.rowlist.setFocus(true)
            handle = true
        else if key = "left"
            if m.rowlist.isInFocusChain() then m.keyboard.setFocus(true)
            handle = true
        else if key = "back"
            m.rowlist.animateToItem = 0
            handle = true
        end if
    end if
    return handle
end function