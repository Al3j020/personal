function parseData(data as object)
    items = []
    if data.results <> invalid
        for each result in data.results
            item = {
                id: result.id,
                title: result.name,
                voteAverage: right(result.vote_average.toStr(), 3),
                releaseDate: result.first_air_date
                posterUrl: "https://image.tmdb.org/t/p/w500/" + result.poster_path
                data: result
            }
            items.push(item)
        end for
    end if
    m.top.data = {items: items}
end function