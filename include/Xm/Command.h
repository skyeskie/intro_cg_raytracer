/**
 *
 * $Id: Command.h,v 1.2 2000/09/23 14:51:40 amai Exp $
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

#ifndef _XM_COMMAND_H
#define _XM_COMMAND_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

extern WidgetClass xmCommandWidgetClass;

typedef struct _XmCommandRec *XmCommandWidget;
typedef struct _XmCommandClassRec *XmCommandClass;

extern Widget XmCreateCommand(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern Widget XmCreateCommandDialog(Widget parent, char *name, Arg *arglist, Cardinal argcount);
extern void XmCommandSetValue(Widget widget, XmString command);
extern void XmCommandAppendValue(Widget widget, XmString command);
extern void XmCommandError(Widget widget, XmString error);
extern Widget XmCommandGetChild(Widget widget, unsigned char child);

#ifndef XmIsCommand
#define XmIsCommand(w) XtIsSubclass((w), xmCommandWidgetClass)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _XM_COMMAND_H */
