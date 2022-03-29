function parseData(data as object)
    items = []
    if data.results <> invalid
        for each result in data.results
            if result.poster_path <> invalid
                item = {
                    id: result.id,
                    title: result.title,
                    releaseDate: result.release_date,
                    voteAverage: right(result.vote_average.toStr(), 3),
                    posterUrl: "https://image.tmdb.org/t/p/w500/"+ result.poster_path
                    data: result
                }
                items.push(item)
            end if
        end for
    end if
    m.top.data = {items: items}
end function