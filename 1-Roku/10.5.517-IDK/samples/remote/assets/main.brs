' Copyright 2021 Roku, Inc.  All rights reserved.
sub Main()
	idk = CreateObject("roIDK")
	retval = idk.start("pkg:/sample-remote", [])
	print "returned: "; retval
end sub
