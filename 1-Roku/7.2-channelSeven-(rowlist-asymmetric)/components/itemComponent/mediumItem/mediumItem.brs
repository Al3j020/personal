sub init()
    m.poster = m.top.findNode("poster")
end sub

sub onItemContent(event as object)
    itemContent = event.getData()
    m.poster.uri = itemContent.hdPosterUrl
end sub