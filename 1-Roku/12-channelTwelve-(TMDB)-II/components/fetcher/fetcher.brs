sub init()
    bindComponents()
    urlMapper()
end sub

sub bindComponents()
    m.httpTask = createObject("roSGNode", "httpTask")
    m.httpTask.control = "RUN"
end sub

function fetchMoviesByGenre(request as object)
    url = substitute("{0}discover/movie?api_key={1}&language=en-US&sort_by=popularity.desc&include_adult=false&include_video=false&page=1&with_genres={2}&with_watch_monetization_types=flatrate", m.baseUrl, m.apiKey, request.genreId.toStr())
    request.addReplace("url", url)
    request.addReplace("model", "moviesModel")
    enqueue(request)
end function

function fetchTVShowsByGenre(request as object)
    url = substitute("{0}discover/tv?api_key={1}&language=en-US&sort_by=popularity.desc&timezone=America%2FNew_York&with_genres={2}&include_null_first_air_dates=false&with_watch_monetization_types=flatrate&with_status=0&with_type=0", m.baseUrl, m.apiKey, request.genreId.toStr())
    request.addReplace("url", url)
    request.addReplace("model", "showsModel")
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