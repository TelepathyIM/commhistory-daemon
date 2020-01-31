/******************************************************************************
**
** This file is part of commhistory-daemon.
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

#include "telepathyhandler.h"
#include "eventsmonitor.h"

TelepathyHandler::TelepathyHandler(QObject *parent, EventsMonitor *eventsMonitor)
    : QObject(parent)
{
    connect(eventsMonitor, &EventsMonitor::eventsUpdated, this, &TelepathyHandler::onEventsUpdated);
    connect(eventsMonitor, &EventsMonitor::groupsUpdatedFull, this, &TelepathyHandler::onGroupsUpdatedFull);
}

void TelepathyHandler::onEventsUpdated(const QList<CommHistory::Event> &events)
{
    Q_UNUSED(events);
}

void TelepathyHandler::onGroupsUpdatedFull(const QList<CommHistory::Group> &groups)
{
    Q_UNUSED(groups);
}
