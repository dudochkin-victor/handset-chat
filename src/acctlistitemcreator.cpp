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


#include "acctlistitemcreator.h"

#include <QModelIndex>
#include <MWidget>

#include <libmeegochat/meegochataccount.h>

void AcctListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!index.isValid() || !cell)
        return;
    AcctListItem *acctListItem = qobject_cast<AcctListItem *>(cell);

    QVariant data = index.data(Qt::UserRole);
    MeeGoChat::Acct *acct = data.value<Acct *>();

    acctListItem->setAccount(acct);
}
