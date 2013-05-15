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


#include "contactlistitemcreator.h"
#include <libmeegochat/meegochatcontact.h>

void ContactListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!index.isValid() || !cell)
        return;
    ContactListItem *contactListItem = qobject_cast<ContactListItem *>(cell);

    QVariant data = index.data(Qt::UserRole);//ChatContact *
    MeeGoChat::ChatContact *contact = data.value<MeeGoChat::ChatContact *>();

    contactListItem->setContact(contact);
}
