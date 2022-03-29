' Copyright 2021 Roku, Inc.  All rights reserved.
sub Main()
	idk = CreateObject("roIDK")
	retval = idk.start("pkg:/sample-video", ["/pkg_/streams/video_encrypted.mp4", "/pkg_/streams/audio_encrypted.mp4"])
	print "returned: "; retval
end sub
