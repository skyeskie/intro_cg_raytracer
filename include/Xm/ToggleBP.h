/**
 *
 * $Id: ToggleBP.h,v 1.7 2000/09/23 14:51:41 amai Exp $
 *
 * Copyright (C) 1995 Free Software Foundation, Inc.
 * Copyright (C) 1995-2000 LessTif Development Team
 *
 * This file is part of the GNU LessTif Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 **/


#ifndef _XM_TOGGLEBP_H
#define _XM_TOGGLEBP_H

#include <Xm/ToggleB.h>
#include <Xm/LabelP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char ind_type;
    Boolean visible;
    Dimension spacing;
    Dimension indicator_dim;
    Boolean indicator_set;
    Pixmap on_pixmap;
    Pixmap insen_pixmap;
    Boolean set;		/* button state */
    Boolean visual_set;		/* visual state */
    Boolean ind_on;		/* do we draw the indicator? */
    unsigned char fill_on_select;
    Pixel select_color;

    GC select_GC;

    GC background_gc;
    XtCallbackList value_changed_CB;
    XtCallbackList arm_CB;
    XtCallbackList disarm_CB;
    Boolean Armed;
} XmToggleButtonPart;

/* Define the full instance record */
typedef struct _XmToggleButtonRec {
    CorePart core;
    XmPrimitivePart primitive;
    XmLabelPart label;
    XmToggleButtonPart toggle;
} XmToggleButtonRec;

/* Define class part structure */
typedef struct {
    XtPointer extension;
} XmToggleButtonClassPart;

/* Define the full class record */
typedef struct _XmToggleButtonClassRec {
    CoreClassPart core_class;
    XmPrimitiveClassPart primitive_class;
    XmLabelClassPart label_class;
    XmToggleButtonClassPart toggle_class;
} XmToggleButtonClassRec;

/* External definition for the class record */

extern XmToggleButtonClassRec xmToggleButtonClassRec;

#ifdef __cplusplus
}
#endif

#endif /* _XM_TOGGLEBP_H */
