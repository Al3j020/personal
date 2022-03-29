sub init()
    bindComponents()
    setStyles()
    setObservers()
end sub

sub bindComponents()
    m.titleLabel = m.top.createChild("label")
    m.poster = m.top.createChild("poster")
    m.buttonGroup = m.top.createChild("buttonGroup")
    m.descriptionLabel = m.top.createChild("label")
    m.metadataLabel = m.top.createChild("label")

    m.metadataLayout = m.top.createChild("layoutGroup")
    m.metadataLayout.insertChildren([m.buttonGroup, m.metadataLabel, m.descriptionLabel], 0)

    m.baseLayout = m.top.createChild("layoutGroup")
    m.baseLayout.insertChildren([m.poster, m.metadataLayout], 0)

    m.mainLayout = m.top.createChild("layoutGroup")
    m.mainLayout.insertChildren([m.titleLabel, m.baseLayout], 0)

    m.top.appendChild(m.mainLayout)
end sub

sub setStyles()
    styles = m.global.styles
    m.mainLayout.setFields(styles.detailsSceneFields.mainLayoutFields)
    m.baseLayout.setFields(styles.detailsSceneFields.baseLayoutFields)
    m.metadataLayout.setFields(styles.detailsSceneFields.metadataLayoutFields)

    m.titleLabel.setFields(styles.detailsSceneFields.labelFields)
    m.poster.setFields(styles.detailsSceneFields.posterFields)
    m.buttonGroup.setFields(styles.detailsSceneFields.buttonGroupFields)
    m.titleLabel.setFields(styles.detailsSceneFields.titleLabelFields)
    m.descriptionLabel.setFields(styles.detailsSceneFields.descriptionLabelFields)
    m.metadataLabel.setFields(styles.detailsSceneFields.metadataLabelFields)
end sub

sub setObservers()
    m.top.observeField("focusedChild", "onFocusChild")
end sub

sub onFocusChild(event as object)
    if m.top.hasFocus()
        m.buttonGroup.setFocus(true)
    end if
end sub

sub onContentChange(event as object)
    content = event.getData()
    m.titleLabel.text = content.title
    m.descriptionLabel.text = content.description
    m.metadataLabel.text = substitute("Release Date: {0} | Vote Average: {1}",content.releaseDate, content.voteAverage)
    m.poster.uri = content.posterUrl
end sub