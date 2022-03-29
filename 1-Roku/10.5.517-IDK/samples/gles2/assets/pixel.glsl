// Copyright 2021 Roku, Inc.  All rights reserved.
precision mediump float; // highp not supported in pixel shader.
uniform bool u_blur;
uniform bool u_mask;
uniform bool u_final;
uniform sampler2D u_tex_mask;
uniform sampler2D u_tex_blur;
uniform sampler2D u_tex_color;
varying vec4 v_color;
varying vec2 v_uv;

void main()
{
	if (u_blur) {
		// Used by RenderBlurry() in starfield.cpp.
		float c = 0.001;
		vec4 tl = texture2D(u_tex_color, vec2(v_uv.x - c, v_uv.y + c));
		vec4 tr = texture2D(u_tex_color, vec2(v_uv.x + c, v_uv.y + c));
		vec4 bl = texture2D(u_tex_color, vec2(v_uv.x - c, v_uv.y - c));
		vec4 br = texture2D(u_tex_color, vec2(v_uv.x + c, v_uv.y - c));
		float c2 = 0.0015;
		vec4 tl2 = texture2D(u_tex_color, vec2(v_uv.x - c2, v_uv.y + c2));
		vec4 tr2 = texture2D(u_tex_color, vec2(v_uv.x + c2, v_uv.y + c2));
		vec4 bl2 = texture2D(u_tex_color, vec2(v_uv.x - c2, v_uv.y - c2));
		vec4 br2 = texture2D(u_tex_color, vec2(v_uv.x + c2, v_uv.y - c2));
		vec4 color = (tl + tr + bl + br + tl2 + tr2 + bl2 + br2) * 0.125;
		gl_FragColor = color * 0.38;
	}
	else if (u_mask) {
		// Used by RenderTextures() in starfield.cpp.
		float a = texture2D(u_tex_mask, v_uv).r;
		gl_FragColor = v_color * a;
	}
	else if (u_final) {
		// Used by RenderAndPresent() in starfield.cpp.
		vec4 color = texture2D(u_tex_color, v_uv);
		vec4 blurry = texture2D(u_tex_blur, v_uv);
		float ratio = texture2D(u_tex_mask, v_uv).r;
		gl_FragColor = (color * ratio) + (blurry * (1.0 - ratio));
	}
	else {
		// Used by RenderColor() in starfield.cpp.
		gl_FragColor = texture2D(u_tex_color, v_uv);
	}
}
