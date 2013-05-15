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


#include "acctlistitem.h"

#include <QGraphicsSceneMouseEvent>

/*
#include <QPixmap>
#include <QImage>
#include <QTransform>
 */
#include <MImageWidget>
#include <MWidgetCreator>
#include <MLinearLayoutPolicy>

#include <libmeegochat/meegochataccount.h>

M_REGISTER_WIDGET(AcctListItem);

#define IMAGE_ID "icon-m-common-drilldown-arrow"

//static QPixmap *gDrillDownRotated = 0;

AcctListItem::AcctListItem(MWidget *parent) :
    MStylableWidget(parent),
    mPressed(false),
    mAcct(0)
{
    /*
     //Can't seem to get the rotation to end up looking right-
     //either I get a black background instead of transparent,
     //or I get no visual at all... will have to revisit for 1.2
    if (!gDrillDownRotated) {
//        QTransform transform;
        QImage *drilldown = new QImage(MTheme::pixmap(IMAGE_ID)->toImage());
        QImage *rotated = new QImage(drilldown->size(), QImage::Format_ARGB32_Premultiplied);
        //rotated->fill(0);


        QPainter painter(rotated);
        QSize size = drilldown->size();

        painter.translate(size.height()/2, size.width()/2);
        painter.rotate(90);
        painter.translate(-size.height()/2, -size.width()/2);
        painter.drawImage(0, 0, *drilldown);
        //painter.drawPixmap(0, 0, *drilldown);
        painter.end();


        gDrillDownRotated = new QPixmap();
        gDrillDownRotated->fromImage(*rotated);
        //gDrillDownRotated = new QPixmap(drilldown->transformed(transform.rotate(90)));
    }
    */
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setObjectName("acctListItem");
    MLayout *hLayout = new MLayout();
    MLinearLayoutPolicy *hPolicy = new MLinearLayoutPolicy(hLayout,
                                                              Qt::Horizontal);

    mDropdown = new MImageWidget();
    mDropdown->setImage(IMAGE_ID);
    //mDropdown->setPixmap(*gDrillDownRotated);
    mDropdown->setObjectName("imgAcctListItemDropdown");
    mDropdown->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hPolicy->addItem(mDropdown, Qt::AlignLeft | Qt::AlignVCenter);

    MImageWidget *vSep1 = new MImageWidget();
    vSep1->setImage("meegotouch-separator-background-vertical");
    vSep1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    hPolicy->addItem(vSep1);

    mAcctName = new MLabel("");
    mAcctName->setTextElide(true);
    mAcctName->setObjectName("acctListItemName");
    mAcctName->setSizePolicy(QSizePolicy::Fixed,
                                QSizePolicy::Fixed);


    hPolicy->addItem(mAcctName, Qt::AlignLeft | Qt::AlignVCenter);

    MImageWidget *vSep2 = new MImageWidget();
    vSep2->setImage("meegotouch-separator-background-vertical");
    vSep2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    //hPolicy->addItem(vSep2);

    mAddContact = new MButton("icon-m-common-add-contact", "");
    mAddContact->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //% "Add Contact"
    mAddContact->setToolTip(qtTrId("ttip_add_contact"));
    mAddContact->setObjectName("btnAcctListItemAddContact");

    connect(mAddContact,
            SIGNAL(clicked()),
            this,
            SLOT(onAddContactClicked()));

    hPolicy->addItem(mAddContact, Qt::AlignLeft | Qt::AlignVCenter);
    //mAddContact->hide();

    hLayout->setPolicy(hPolicy);

    setLayout(hLayout);
    resizeAcctName();

}

void AcctListItem::setAccount(Acct *acct)
{
    mAcct = acct;
    if (mAcct)
        setName(mAcct->getDisplayName());
}

Acct * AcctListItem::getAccount()
{
    return mAcct;
}


void AcctListItem::setName(QString acctName)
{
    mAcctName->setText(acctName);
}

void AcctListItem::setExpanded(bool expanded)
{

    if (expanded)
    {
        //mAddContact->show();
        //mDropdown->setPixmap(*gDrillDownRotated);
        //mDropdown->setImage("icon-m-framework-back");
    } else
    {
        //mAddContact->hide();
       // mDropdown->setImage(IMAGE_ID);
    }
}

void AcctListItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    mPressed = true;
    style().setModePressed();
    update();
    ev->accept();
}

void AcctListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (mPressed) {
        mPressed = false;
        style().setModeDefault();
        update();
        ev->accept();
        emit clicked();
    } else {
        ev->ignore();
    }
}

void AcctListItem::cancelEvent(MCancelEvent *ev)
{
    mPressed = false;
    style().setModeDefault();
    update();
    ev->accept();
}

void AcctListItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MStylableWidget::resizeEvent(event);
    this->resizeAcctName();
}

void AcctListItem::resizeAcctName()
{
    /*qreal nw = this->size().width();
    nw -= mAddContact->size().width();
    nw -= mDropdown->size().width();
    //nw -= 25;

    mAcctName->setMinimumWidth(nw);
    mAcctName->setMaximumWidth(nw);
    mAcctName->setPreferredWidth(nw);*/
}

void AcctListItem::onAddContactClicked()
{
    emit AddContactClicked(mAcct);
}
