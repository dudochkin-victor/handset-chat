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



#ifndef CONTACTMESSAGELISTITEMCREATOR_H
#define CONTACTMESSAGELISTITEMCREATOR_H

#include <MAbstractCellCreator>

#include "contactmessagelistitem.h"

class ContactMessageListItemCreator : public MAbstractCellCreator<ContactMessageListItem>
{
public:
    void updateCell(const QModelIndex &, MWidget *) const;
};

#endif // CONTACTMESSAGELISTITEMCREATOR_H
