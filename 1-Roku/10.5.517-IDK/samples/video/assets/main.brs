' Copyright 2021 Roku, Inc.  All rights reserved.
sub Main()
	idk = CreateObject("roIDK")
	retval = idk.start("pkg:/sample-video", ["file://pkg_/streams/video.mp4", "file://pkg_/streams/audio.ts"])
	print "returned: "; retval
end sub
