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


#include "contactmessagelistitemcreator.h"

#include <libmeegochat/meegochatcontact.h>

using namespace MeeGoChat;

void ContactMessageListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!index.isValid() || !cell)
        return;
    ContactMessageListItem *contactMessageListItem = qobject_cast<ContactMessageListItem *>(cell);

    QVariant data = index.data(Qt::UserRole);//ChatContact *
    ChatContact *contact = data.value<ChatContact *>();

    contactMessageListItem->setContact(contact);
}
