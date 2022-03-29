sub init()
    m.top.functionName = "startTTSTask"
    m.port = createObject("roMessagePort")
    m.TTS = createObject("roTextToSpeech")
    m.TTS.setMessagePort(m.port)
    m.top.observeField("node", m.port)
    m.top.observeField("textToSpeech", m.port)
end sub

sub startTTSTask()
    while true
        event = wait(0, m.port)
        if type(event) = "roSGNodeEvent"
            parseEvent(event)
        else if type(event) = "roTextToSpeechEvent"
        end if
    end while
end sub

sub parseEvent(event as Object)
    ?"[parseNode]"
    textToSpeech = event.getData()
    m.TTS.say(textToSpeech)

    ' node = event.getData()
    ' if node.subtype() = "ButtonGroup"
    '     buttonFocused = node.buttonFocused
    '     textToSpeech = node.buttons[buttonFocused]
    '     m.TTS.say(textToSpeech)
    ' end if
end sub