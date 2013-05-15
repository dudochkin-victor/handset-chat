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


#include "messagelistitemcreator.h"

#include <libmeegochat/meegochatmessage.h>

using namespace MeeGoChat;

void MessageListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!index.isValid() || !cell)
        return;
    cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    MessageListItem *messageListItem = qobject_cast<MessageListItem *>(cell);

    Message *msg = index.data(Qt::UserRole).value<Message *>();
    messageListItem->setMsg(msg);

//TODO-UI-LT:  turns out this is a limitation of the MFastList right now...
//    - figure out why multi-line is overlapping adjacent cells...
}
