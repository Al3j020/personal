// Copyright 2021 Roku, Inc.  All rights reserved.
attribute vec3 a_pos;
attribute vec4 a_color;
attribute vec2 a_uv;
uniform mat4 u_mvp;
varying vec4 v_color;
varying vec2 v_uv;

void main()
{
	// GL texture coordinate starts from lower-left corder, but images are encoded
	// top to bottom.  Here we flip the texture coordinate vertically to deal with
	// that.
	v_uv = vec2(a_uv.x, 1.0 - a_uv.y);
	v_color = a_color;
	gl_Position = vec4(a_pos, 1.0) * u_mvp;
}
