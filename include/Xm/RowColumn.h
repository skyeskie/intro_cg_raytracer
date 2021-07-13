/**
 * 
 * $Id: RowColumn.h,v 1.2 2000/09/23 14:51:40 amai Exp $
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


#ifndef _XM_ROWCOLUMN_H
#define _XM_ROWCOLUMN_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

extern WidgetClass xmRowColumnWidgetClass;

typedef struct _XmRowColumnRec *XmRowColumnWidget;
typedef struct _XmRowColumnClassRec *XmRowColumnWidgetClass;

#ifndef XmIsRowColumn
#define XmIsRowColumn(w) XtIsSubclass((w), xmRowColumnWidgetClass)
#endif

extern Widget XmCreateMenuBar(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreateOptionMenu(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreatePopupMenu(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreatePulldownMenu(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreateRadioBox(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreateRowColumn(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreateWorkArea(Widget parent, char *name, Arg *arglist, Cardinal argcount);

extern Widget XmOptionButtonGadget(Widget option_menu);
extern Widget XmOptionLabelGadget(Widget option_menu);

extern void XmMenuPosition(Widget menu, XButtonPressedEvent *event);
extern Widget XmGetTearOffControl(Widget menu);

extern void XmAddToPostFromList(Widget menu_wid, Widget widget);
extern void XmRemoveFromPostFromList(Widget menu_wid, Widget widget);
extern Widget XmGetPostedFromWidget(Widget menu);

#ifdef __cplusplus
}
#endif

#endif /* _XM_ROWCOLUMN_H */
