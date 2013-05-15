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



#ifndef MESSAGELISTITEMCREATOR_H
#define MESSAGELISTITEMCREATOR_H

#include <MAbstractCellCreator>

#include "messagelistitem.h"

class MessageListItem;

class MessageListItemCreator : public MAbstractCellCreator<MessageListItem>
{
public:
    void updateCell(const QModelIndex &, MWidget *) const;
};

#endif // MESSAGELISTITEMCREATOR_H
