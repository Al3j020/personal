/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2008-2013 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/**
 * @file fbdev_window.h
 * @brief A window type for the framebuffer device (used by egl and tests)
 */

#ifndef _FBDEV_WINDOW_H_
#define _FBDEV_WINDOW_H_

#include "directfb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	FBDEV_PIXMAP_DEFAULT = 0,
	FBDEV_PIXMAP_ALPHA_FORMAT_PRE = (1<<0),
	FBDEV_PIXMAP_COLORSPACE_sRGB = (1<<1),
	FBDEV_PIXMAP_EGL_MEMORY = (1<<2),       /* EGL allocates/frees this memory */
} fbdev_pixmap_flags;

typedef enum
{
    FBDEV_NULL = (0),
	FBDEV_WIDTH = (1<<0),
	FBDEV_HEIGHT = (1<<1),
	FBDEV_NATIVE_WINDOW = (1<<2),
	FBDEV_NATIVE_SURFACE = (1<<3),
	
    FBDEV_WIDTH_HEIGHT = (FBDEV_WIDTH | FBDEV_HEIGHT),
    FBDEV_NATIVE_WINDOW_SURFACE = (FBDEV_NATIVE_WINDOW | FBDEV_NATIVE_SURFACE),
    FBDEV_ALL = (FBDEV_WIDTH_HEIGHT | FBDEV_NATIVE_WINDOW_SURFACE),
} fbdev_window_flags;

typedef struct fbdev_window
{      
    fbdev_window_flags flags;
    u16 width;
    u16 height;
    IDirectFBWindow *window;
    IDirectFBSurface *surface;
} fbdev_window;

typedef struct fbdev_pixmap
{
    fbdev_pixmap_flags flags;
    IDirectFBSurface *surface;
} fbdev_pixmap;


#ifdef __cplusplus
}
#endif


#endif
