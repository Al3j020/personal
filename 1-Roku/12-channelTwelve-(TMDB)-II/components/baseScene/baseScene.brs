sub init()
    bindComponents()
    setObservers()
    setStyles()
end sub

sub bindComponents()
    m.top.fetcher = createObject("roSGNode","fetcher")
    m.topBar = m.top.createChild("rowlist")
    m.rowlist = m.top.createChild("rowlist")
    m.firstFocus = false
    m.counter = 0
    m.rows = []
end sub

sub setObservers()
    m.top.observeField("focusedChild", "onFocusedChild")
    m.topBar.observeField("rowItemFocused", "onTopBarItemFocused")
    m.rowlist.observeField("rowItemSelected", "onRowItemSelected")
    m.rowlist.observeField("rowItemFocused", "onRowItemFocused")
end sub

sub setStyles()
    m.topBar.setFields(m.global.styles.topBarFields)
    m.rowlist.setFields(m.global.styles.rowListFields)
end sub

sub onFocusedChild(event as object)
    if m.top.hasFocus()
        if m.firstFocus = false
            m.topBar.setFocus(true)
            m.firstFocus = true
        else
            m.rowlist.setFocus(true)
        end if
    end if
end sub

sub onTopBarItemFocused(event as object)
    rowItemIndex = event.getData()
    rowItem = m.topBar.content.getChild(rowItemIndex[0]).getChild(rowItemIndex[1])
    if rowItem.genre = "All"
        for each genre in rowItem.genres
            fetchMoviesByGenre(genre.genreId)
            fetchTVShowsByGenre(genre.genreId)
        end for
    else
        fetchMoviesByGenre(rowItem.genreId)
        fetchTVShowsByGenre(rowItem.genreId)
    end if
end sub

sub onRowItemSelected(event as object)
    rowItemIndex = event.getData()
    rowItem = m.rowlist.content.getChild(rowItemIndex[0]).getChild(rowItemIndex[1])
    m.global.newSceneCTA = {node:"detailsScene", content: rowItem, childFocus: m.rowlist}
end sub

sub fetchMoviesByGenre(genreId)
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onHttpResponse")
    request = {method: "GET", httpNode: httpNode, genreId: genreId}
    m.top.fetcher.callFunc("fetchMoviesByGenre", request)
    m.counter++
end sub

sub fetchTVShowsByGenre(genreId)
    httpNode = createObject("roSGNode","httpNode")
    httpNode.observeField("response", "onHttpResponse")
    request = {method: "GET", httpNode: httpNode, genreId: genreId}
    m.top.fetcher.callFunc("fetchTVShowsByGenre", request)
    m.counter++
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
    m.rows = invalid
    m.rows = []
end sub

function onKeyEvent(key as string, press as boolean) as boolean
    handle = false
    if press
        if key = "down"
            if m.topBar.hasFocus() then m.rowlist.setFocus(true)
            handle = true
        else if key = "up"
            if m.rowList.hasFocus() then m.topBar.setFocus(true)
            handle = true
        else if key = "back"
            m.rowlist.jumpToItem = 0
            handle = true
        end if
    end if
    return handle
end function