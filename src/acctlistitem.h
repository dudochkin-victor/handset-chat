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


#ifndef ACCTLISTITEM_H
#define ACCTLISTITEM_H

#include <QGraphicsSceneMouseEvent>
#include <MStylableWidget>
#include <MLayout>
#include <MLabel>
#include <MImageWidget>
#include <MButton>
#include <MCancelEvent>

#include "acctlistitemstyle.h"

namespace MeeGoChat {
    class Acct;
}

using namespace MeeGoChat;

class AcctListItem : public MStylableWidget
{
Q_OBJECT
public:
    AcctListItem(MWidget *parent = 0);
    void setAccount(MeeGoChat::Acct *acct);
    void setExpanded(bool expanded);
    Acct * getAccount();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    void cancelEvent(MCancelEvent*);
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
    void onAddContactClicked();

signals:
    void clicked();
    void AddContactClicked(Acct *);

private:
    void setName(QString acctName);
    void resizeAcctName();

    MLayout *mLayout;
    MLabel *mAcctName;
    MImageWidget *mDropdown;
    MButton *mAddContact;
    bool mPressed;
    MeeGoChat::Acct *mAcct;

    M_STYLABLE_WIDGET(AcctListItemStyle);

};

#endif // ACCTLISTITEM_H
