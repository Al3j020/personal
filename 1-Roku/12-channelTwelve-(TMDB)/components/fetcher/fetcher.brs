sub init()
    bindComponents()
    urlMapper()
end sub

sub bindComponents()
    m.httpTask = createObject("roSGNode", "httpTask")
    m.httpTask.control = "RUN"
end sub

function fetchTrending(request as object)
    queryParams = request.queryParams
    url = substitute("{0}trending/{1}/{2}?api_key={3}", m.baseUrl, queryParams.mediaType, queryParams.timeWindow, m.apiKey)
    request.addReplace("url", url)
    request.addReplace("model", "moviesModel")
    enqueue(request)
end function

function fetchTVShows(request as object)
    url = substitute("{0}tv/popular?api_key={1}&language=en-US&page=1", m.baseUrl, m.apiKey)
    request.addReplace("url", url)
    request.addReplace("model", "showsModel")
    enqueue(request)
end function

function fetchRecomendations(request as object)
    url = substitute("{0}tv/{1}/recommendations?api_key={2}&language=en-US&page=1", m.baseUrl, request.showId.toStr(), m.apiKey)
    request.addReplace("url", url)
    request.addReplace("model", "showsModel")
    enqueue(request)
end function

function fetchProviders(request as object)
    url = substitute("https://api.themoviedb.org/3/tv/{0}/watch/providers?api_key=558e2410c2be44f6e971c2b2c8cf64d0&region=CO", request.showId.toStr())
    request.addReplace("url", url)
    request.addReplace("model", "providersModel")
    enqueue(request)
end function

sub enqueue(request as object)
    m.httpTask.request = request
end sub

sub urlMapper()
    appInfo     = createObject("roAppInfo")
    m.baseUrl   = appInfo.getValue("base_url")
    m.apiKey    = appInfo.getValue("api_key")
end sub