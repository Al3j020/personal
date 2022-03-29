' Copyright 2021 Roku, Inc.  All rights reserved.
sub Main()
	idk = CreateObject("roIDK")
	retval = idk.start("pkg:/sample-sfx", [])
	print "returned: "; retval
end sub
