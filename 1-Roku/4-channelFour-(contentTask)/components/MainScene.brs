sub init()
    m.labelList = m.top.findNode("labelList")
    m.labelList.setFocus(true)
    createTask()
end sub

sub createTask()    
    request = {
        genres: [
            {   
                "title"         : "documentary",
                "iconFocused"   : "pkg:/images/check-poster.png",
                "iconUnfocused" : "pkg:/images/star-poster.png"  
            },
            {   "title" : "drama",
                "iconFocused"   : "pkg:/images/check-poster.png",
                "iconUnfocused" : "pkg:/images/star-poster.png"
            },
            {   "title" : "action",
                "iconFocused"   : "pkg:/images/check-poster.png",
                "iconUnfocused" : "pkg:/images/star-poster.png"
            }
            {   "title" : "horror",
                "iconFocused"   : "pkg:/images/check-poster.png",
                "iconUnfocused" : "pkg:/images/star-poster.png"
            }
            {   "title" : "comedy",
                "iconFocused"   : "pkg:/images/check-poster.png",
                "iconUnfocused" : "pkg:/images/star-poster.png"
            },
        ]
    }
   
    m.contentTask = createObject("roSGNode", "contentTask")
    m.contentTask.observeField("response", "onResponseTask")
    m.contentTask.request = request 
    m.contentTask.control = "RUN" 
end sub

sub onResponseTask(event as Object)
    response = event.getData()
    m.labelList.content = response.data
    m.contentTask.control = "STOP"
    m.contentTask = invalid
end sub