sub init()
    _bindComponents()
    _bindObservers()
    _bindConfigs()
end sub

sub _bindComponents()
    m.video = createObject("roSGNode","video")
    m.buttonGroup = createObject("roSGNode","buttonGroup")
    m.top.insertChildren([m.video, m.buttonGroup], 0)
end sub

sub _bindObservers()
    m.video.observeField("state", "onStateChange")
    m.video.observeField("position", "onPositionChange")
    m.buttonGroup.observeField("buttonSelected", "onButtonSelected")
end sub

sub _bindConfigs()
    buttons = ["Play", "Pause", "Resume"]
    m.buttonGroup.buttons = buttons
    ' m.video.setFocus(true) --> if events are controlled by media keys (play, pause, etc) 
    m.buttonGroup.setFocus(true)
    setVideo()
end sub

function setVideo() as void
    videoContent = createObject("RoSGNode", "ContentNode")
    videoContent.url = "https://roku.s.cpl.delvenetworks.com/media/59021fabe3b645968e382ac726cd6c7b/60b4a471ffb74809beb2f7d5a15b3193/roku_ep_111_segment_1_final-cc_mix_033015-a7ec8a288c4bcec001c118181c668de321108861.m3u8"
    videoContent.title = "Test Video"
    videoContent.streamformat = "hls"

    m.video.content = videoContent
    m.video.control = "play"
end function

sub onStateChange(event as Object)
    state = event.getData()
    print(state)
end sub

sub onPositionChange(event as Object)
    position = event.getData()
    print(position)
end sub

sub onButtonSelected(event as Object)
    buttonSelected = event.getData()
    if buttonSelected = 0
        m.video.control = "play"
    else if buttonSelected = 1
        m.video.control = "pause"
    else
        m.video.control = "resume"
    end if
    print(buttonSelected)
end sub