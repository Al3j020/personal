function parseData(data as object)
    items = []
    if data.results <> invalid
        for each result in data.results
            if result.poster_path <> invalid and result.poster_path <> ""
                item = {
                    id: result.id,
                    title: result.name,
                    voteAverage: right(result.vote_average.toStr(), 3),
                    releaseDate: result.first_air_date
                    posterUrl: "https://image.tmdb.org/t/p/w400/" + result.poster_path
                    description: result.overview
                    genreIds : result.genre_ids
                    data: result
                }
                items.push(item)
            end if
        end for
    end if
    m.top.data = {items: items}
end function