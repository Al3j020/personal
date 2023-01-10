sub init()
    m.top.functionName = "getContent"
end sub 

sub getContent()
    urlTransfer = CreateObject("roUrlTransfer")
    urlTransfer.setUrl("http://raw.githubusercontent.com/jack1590/Roku/master/Netflix/mock/profile.json")
    urlTransfer.setCertificatesFile("common:/certs/ca-bundle.crt")
    urlTransfer.initClientCertificates()
    stringBody = urlTransfer.getToString() 
    data = parseJson(stringBody)
 
    content = createObject("RoSGNode", "ContentNode") 
    section = content.createChild("ContentNode")

    for i=0 to data.profile.items.count() - 1
        profile = data.profile.items[i]
        itemContentNode = section.createChild("ContentNode")
        itemContentNode.id = profile.id
        itemContentNode.title = profile.name
        itemContentNode.HDPOSTERURL = profile.poster 
        itemContentNode.addFields({backgroundActive:true})
        itemContentNode.addField("backgroundActive","Boolean",true)
        itemContentNode.setField("backgroundActive",true)

    end for 

    m.top.output = content 
end sub 
 