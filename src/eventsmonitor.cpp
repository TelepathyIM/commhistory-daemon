/******************************************************************************
**
** This file is part of commhistory-daemon.
**
** Copyright (C) 2020 Jolla Ltd.
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License version 2.1 as
** published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
** License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
**
******************************************************************************/

#include "eventsmonitor.h"

#include <CommHistory/event.h>
#include <CommHistory/group.h>
#include <CommHistory/constants.h>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>

EventsMonitor::EventsMonitor(QObject *parent)
    : QObject(parent)
{
    QDBusConnection dbus(QDBusConnection::sessionBus());
    if (!dbus.connect(QString(), COMM_HISTORY_OBJECT_PATH, COMM_HISTORY_INTERFACE,
        EVENTS_UPDATED_SIGNAL, this, SLOT(onDBusEventsUpdated(QList<CommHistory::Event>)))) {
        qWarning() << "EventsMonitor: failed to register" << EVENTS_UPDATED_SIGNAL << "handler";
    }
    if (!dbus.connect(QString(), COMM_HISTORY_OBJECT_PATH, COMM_HISTORY_INTERFACE,
        GROUPS_UPDATED_FULL_SIGNAL, this, SLOT(onDBusGroupsUpdatedFull(QList<CommHistory::Group>)))) {
        qWarning() << "EventsMonitor: failed to register" << GROUPS_UPDATED_FULL_SIGNAL << "handler";
    }
}

void EventsMonitor::onDBusEventsUpdated(const QList<CommHistory::Event> &events)
{
    emit eventsUpdated(events);
}

void EventsMonitor::onDBusGroupsUpdatedFull(const QList<CommHistory::Group> &groups)
{
    emit groupsUpdatedFull(groups);
}
