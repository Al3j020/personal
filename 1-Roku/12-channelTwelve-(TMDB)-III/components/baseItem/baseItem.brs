sub init()
    bindComponents()
    setStyles()
end sub

sub bindComponents()
    m.poster            = createObject("roSGNode","poster")
    m.titleLabel        = createObject("roSGNode", "label")
    m.releaseLabel      = createObject("roSGNode", "label")
    m.metadataLayout    = createObject("roSGNode", "layoutGroup")

    m.metadataLayout.insertChildren([m.titleLabel, m.releaseLabel], 0)
    m.top.insertChildren([m.poster, m.metadataLayout], 0)
end sub

sub setStyles()
    styles = m.global.styles
    m.poster.setFields(styles.baseItemFields.posterFields)
    m.titleLabel.setFields(styles.baseItemFields.labelFields)
    m.releaseLabel.setFields(styles.baseItemFields.labelFields)
    m.metadataLayout.setFields(styles.baseItemFields.metadataLayoutFields)
end sub

sub onItemContent(event as object)
    content = event.getData()
    m.titleLabel.text   = content.title
    m.releaseLabel.text = substitute("{0} | {1}", content.voteAverage, content.releaseDate)
    m.poster.uri        = content.posterUrlHoriz
end sub