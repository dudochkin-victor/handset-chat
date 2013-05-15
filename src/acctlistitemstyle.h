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



#ifndef ACCTLISTITEMSTYLE_H
#define ACCTLISTITEMSTYLE_H

#include <MWidgetStyle>

class M_EXPORT AcctListItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(AcctListItemStyle)

};

class M_EXPORT AcctListItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(AcctListItemStyle);
    M_STYLE_MODE(Read);
    M_STYLE_MODE(Pressed);
};

#endif // ACCTLISTITEMSTYLE_H
