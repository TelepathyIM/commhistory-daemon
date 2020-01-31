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

#ifndef EVENTSMONITOR_H
#define EVENTSMONITOR_H

#include <QObject>

namespace CommHistory {

class Event;
class Group;

} // CommHistory

class EventsMonitor : public QObject
{
    Q_OBJECT

public:
    explicit EventsMonitor(QObject *parent = nullptr);

Q_SIGNALS:
    void eventsUpdated(const QList<CommHistory::Event> &events);
    void groupsUpdatedFull(const QList<CommHistory::Group> &groups);

private Q_SLOTS:
    void onDBusEventsUpdated(const QList<CommHistory::Event> &events);
    void onDBusGroupsUpdatedFull(const QList<CommHistory::Group> &groups);

};

#endif //EVENTSMONITOR_H
