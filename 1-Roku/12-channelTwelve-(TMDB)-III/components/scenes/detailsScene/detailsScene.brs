sub init()
    bindComponents()
    setStyles()
    setObservers()
end sub

sub bindComponents()
    m.titleLabel = createObject("roSGNode","label")
    m.poster = createObject("roSGNode","poster")
    m.buttonGroup = createObject("roSGNode","buttonGroup")
    m.descriptionLabel = createObject("roSGNode","label")
    m.metadataLabel = createObject("roSGNode","label")

    m.metadataLayout = createObject("roSGNode","layoutGroup")
    m.metadataLayout.insertChildren([m.buttonGroup, m.metadataLabel, m.descriptionLabel], 0)

    m.baseLayout = createObject("roSGNode","layoutGroup")
    m.baseLayout.insertChildren([m.poster, m.metadataLayout], 0)

    m.mainLayout = createObject("roSGNode","layoutGroup")
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
    m.buttonGroup.observeField("buttonSelected", "onButtonSelected")
end sub

sub onFocusChild(event as object)
    if m.top.hasFocus()
        m.buttonGroup.setFocus(true)
    end if
end sub

sub onButtonSelected(event as object)
    rootScene = m.top.getScene()
    rootScene.router = {scene : "videoScene"}
end sub

sub addContent(event as object)
    content = event.getData()
    m.titleLabel.text = content.title
    m.descriptionLabel.text = content.description
    m.metadataLabel.text = substitute("Release Date: {0} | Vote Average: {1}",content.releaseDate, content.voteAverage)
    m.poster.uri = content.posterUrl
end sub