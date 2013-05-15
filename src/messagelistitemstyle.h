/*
 * meego-handset-chat - Meego Handset Chat application
 *
 * Copyright (c) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */



#ifndef MESSAGELISTITEMSTYLE_H
#define MESSAGELISTITEMSTYLE_H

#include <MWidgetStyle>

class M_EXPORT MessageListItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MessageListItemStyle)

};

class M_EXPORT MessageListItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MessageListItemStyle);
    M_STYLE_MODE(Incoming);
    M_STYLE_MODE(Outgoing);
    M_STYLE_MODE(Pressed);
    M_STYLE_MODE(PendingSend);
};

#endif // MESSAGELISTITEMSTYLE_H
