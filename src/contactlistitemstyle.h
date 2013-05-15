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



#ifndef CONTACTLISTITEMSTYLE_H
#define CONTACTLISTITEMSTYLE_H

#include <QObject>
#include <MStyle>
#include <MWidgetStyle>

class M_EXPORT ContactListItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(ContactListItemStyle)

};

class M_EXPORT ContactListItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(ContactListItemStyle);
    M_STYLE_MODE(Read);
    M_STYLE_MODE(Pressed);
};

#endif // CONTACTLISTITEMSTYLE_H
