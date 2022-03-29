' Copyright 2021 Roku, Inc.  All rights reserved.
sub Main()
	device = CreateObject("roDeviceInfo")
	display = device.GetDisplaySize()
	width = display.w
	height = display.h

	idk = CreateObject("roIDK")
	retval = idk.start("pkg:/sample-gles2", [width.ToStr(), height.ToStr()])
	print "returned: "; retval
end sub
