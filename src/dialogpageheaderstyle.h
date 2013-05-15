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



#ifndef DIALOGPAGEHEADERSTYLE_H
#define DIALOGPAGEHEADERSTYLE_H

#include <MWidgetStyle>

class M_EXPORT DialogPageHeaderStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(DialogPageHeaderStyle)

};

class M_EXPORT DialogPageHeaderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(DialogPageHeaderStyle);
};

#endif // DIALOGPAGEHEADERSTYLE_H
