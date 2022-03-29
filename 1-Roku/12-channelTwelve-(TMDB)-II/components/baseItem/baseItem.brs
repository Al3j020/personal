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
    m.poster.setFields(styles.baseItemFields.posterFields)
    m.rectangle.setFields(styles.baseItemFields.rectangleFields)
    m.titleLabel.setFields(styles.baseItemFields.labelFields)
    m.releaseLabel.setFields(styles.baseItemFields.labelFields)
    m.scoreLabel.setFields(styles.baseItemFields.labelFields)
    m.baseLayout.setFields(styles.baseItemFields.baseLayoutFields)
    m.metadataLayout.setFields(styles.baseItemFields.metadataLayoutFields)
end sub

sub onItemContent(event as object)
    content = event.getData()
    m.titleLabel.text   = content.title
    m.releaseLabel.text = content.releaseDate
    m.scoreLabel.text   = content.voteAverage
    m.poster.uri        = content.posterUrl
end sub