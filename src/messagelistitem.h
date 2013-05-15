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



#ifndef MESSAGELISTITEM_H
#define MESSAGELISTITEM_H

#include <QGraphicsSceneMouseEvent>
#include <MStylableWidget>
#include <MListItem>
#include <MCancelEvent>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>

#include <libmeegochat/meegochatmessage.h>

#include "timeutil.h"
#include "messagelistitemstyle.h"

using namespace MeeGoChat;

class MessageListItem : public MListItem
{
Q_OBJECT
public:
    MessageListItem(MWidget *parent = 0);
    void setMsg(Message *msg);
    Message * getMsg();

signals:
    void clicked();
    void deleteMe(MessageListItem *);
    void deleteAll();

private slots:
    void deleteMessage();
    void deleteThread();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    void cancelEvent(MCancelEvent*);

private:
    MLayout *mLayout;
    MLinearLayoutPolicy *mPolicy;
    MLabel *mMsgText;
    MLabel *mMsgRcvd;
    Message *mMsg;
    bool mShowTime;
    bool mPressed;

    //M_STYLABLE_WIDGET(MessageListItemStyle);
};

#endif // MESSAGELISTITEM_H
