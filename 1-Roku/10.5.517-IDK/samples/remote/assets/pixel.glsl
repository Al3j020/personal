// Copyright 2021 Roku, Inc.  All rights reserved.
precision mediump float; // highp not supported in pixel shader.
uniform float u_stuff;

void main()
{
	gl_FragColor = vec4(u_stuff, 0.0, 0.0, 1.0);
}
