function consoleLog(msg)
    if type(msg.info) = "roAssociativeArray"
        print ">[info] "; formatJson(msg.info)
    else
        print ">[info] "; msg.info
    end if
end function