function parseData(data as object)
    items = []
    if data.results <> invalid
        for each result in data.results
            posterPathVert = ""
            posterPathHoriz = ""
            if result.poster_path <> invalid and result.poster_path <> "" then  posterPathVert = result.poster_path
            if result.backdrop_path <> invalid and result.backdrop_path <> "" then   posterPathHoriz = result.backdrop_path

            if posterPathVert <> "" and posterPathHoriz <> ""
                item = {
                    id: result.id,
                    title: result.title,
                    releaseDate: result.release_date,
                    voteAverage: right(result.vote_average.toStr(), 3),
                    posterUrlVert:  "https://image.tmdb.org/t/p/w400" + posterPathVert
                    posterUrlHoriz: "https://image.tmdb.org/t/p/w400" + posterPathHoriz
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