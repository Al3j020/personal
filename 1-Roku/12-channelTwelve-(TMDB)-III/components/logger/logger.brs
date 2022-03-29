function consoleLog(msg)
    ' print ">[info] ", formatJson(msg.info)
    if type(msg.info) = "roAssociativeArray" or  type(msg.info) = "roArray"
        print ">[info] "; formatJson(msg.info)
    else
        print ">[info] "; msg.info
    end if
end function