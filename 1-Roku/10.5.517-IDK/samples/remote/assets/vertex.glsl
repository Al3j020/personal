// Copyright 2021 Roku, Inc.  All rights reserved.
attribute vec3 a_pos;
uniform mat4 u_mvp;

void main()
{
	gl_Position = vec4(a_pos, 1.0) * u_mvp;
}
