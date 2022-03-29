sub init()
    bindComponents()
    setStyles()
end sub

sub bindComponents()
    m.poster            = createObject("roSGNode","poster")
    m.rectangle         = createObject("roSGNode","rectangle")
    m.titleLabel        = createObject("roSGNode", "label")
    m.releaseLabel      = createObject("roSGNode", "label")
    m.scoreLabel        = createObject("roSGNode", "label")
    m.baseLayout        = createObject("roSGNode", "layoutGroup")
    m.metadataLayout    = createObject("roSGNode", "layoutGroup")

    m.top.appendChild(m.baseLayout)
    m.rectangle.appendChild(m.metadataLayout)
    m.baseLayout.insertChildren([m.poster, m.rectangle], 0)
    m.metadataLayout.insertChildren([m.titleLabel, m.releaseLabel, m.scoreLabel], 0)
end sub

sub setStyles()
    styles = m.global.styles
    m.poster.setFields(styles.posterFields)
    m.rectangle.setFields(styles.rectangleFields)
    m.titleLabel.setFields(styles.labelFields)
    m.releaseLabel.setFields(styles.labelFields)
    m.scoreLabel.setFields(styles.labelFields)
    m.baseLayout.setFields(styles.baseLayoutFields)
    m.metadataLayout.setFields(styles.metadataLayoutFields)
end sub

sub onItemContent(event as object)
    content = event.getData()
    m.titleLabel.text   = content.title
    m.releaseLabel.text = content.releaseDate
    m.scoreLabel.text   = content.voteAverage
    m.poster.uri        = content.posterUrl
end sub