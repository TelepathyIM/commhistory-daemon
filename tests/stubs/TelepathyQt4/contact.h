/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2008 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2008 Nokia Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _TelepathyQt4_contact_h_HEADER_GUARD_
#define _TelepathyQt4_contact_h_HEADER_GUARD_

#include "Types"

#include <QObject>
#include <QSet>
#include <QSharedPointer>
#include <QVariantMap>

namespace Tp
{

class ContactCapabilities;
class ContactLocation;
class ContactManager;
class PendingContactInfo;
class PendingOperation;
class Presence;
class ReferencedHandles;

class Contact : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Contact);

public:
    enum Feature {
        FeatureAlias,
        FeatureAvatarToken,
        FeatureSimplePresence,
        FeatureCapabilities,
        FeatureLocation,
        FeatureInfo,
        FeatureAvatarData,
        _Padding = 0xFFFFFFFF
    };

    enum PresenceState {
         PresenceStateNo,
         PresenceStateAsk,
         PresenceStateYes
    };

    ContactManager *manager() const;

    ReferencedHandles handle() const;
    QString id() const; // TODO filter: exact, prefix, substring match

    QSet<Feature> requestedFeatures() const;
    QSet<Feature> actualFeatures() const;

    QString alias() const; // TODO filter: exact, prefix, substring match

    bool isAvatarTokenKnown() const;
    QString avatarToken() const;

    Presence presence() const;

    // TODO filter: the same as Account filtering by caps
    // FIXME: (API/ABI break) Return ContactCapabilities
    ContactCapabilities *capabilities() const;

    // TODO filter: is it available, how accurate, are they near me
    // FIXME: (API/ABI break) Return ContactLocation
    ContactLocation *location() const;

    // TODO filter: having a specific field, having ANY field,
    // (field: exact, contents: exact/prefix/substring)
    PendingOperation *refreshInfo();
    PendingContactInfo *requestInfo();

    /*
     * Filters on exact values of these, but also the "in your contact list at all or not" usecase
     */
    PresenceState subscriptionState() const;
    PresenceState publishState() const;

    PendingOperation *requestPresenceSubscription(const QString &message = QString());
    PendingOperation *removePresenceSubscription(const QString &message = QString());
    PendingOperation *authorizePresencePublication(const QString &message = QString());
    PendingOperation *removePresencePublication(const QString &message = QString());

    /*
     * Filter on being blocked or not
     */
    bool isBlocked() const;
    PendingOperation *block(bool value = true);

    /*
     * Filter on the groups they're in - to show a specific group only
     *
     * Also prefix/substring match on ANY of the groups of the contact
     */
    QStringList groups() const;
    PendingOperation *addToGroup(const QString &group);
    PendingOperation *removeFromGroup(const QString &group);

    ~Contact();

Q_SIGNALS:
    void aliasChanged(const QString &alias);

    void avatarTokenChanged(const QString &avatarToken);

    // FIXME: (API/ABI break) Remove simplePresenceChanged in favor of presenceChanged
    void simplePresenceChanged(const QString &status, uint type, const QString &presenceMessage);
    void presenceChanged(const Tp::Presence &presence);

    // FIXME: (API/ABI break) Use Tp::ContactCapabilities
    void capabilitiesChanged(Tp::ContactCapabilities *caps);

    // FIXME: (API/ABI break) Use Tp::ContactLocation
    void locationUpdated(Tp::ContactLocation *location);

    void subscriptionStateChanged(Tp::Contact::PresenceState state);
    void publishStateChanged(Tp::Contact::PresenceState state);
    void blockStatusChanged(bool blocked);

    void addedToGroup(const QString &group);
    void removedFromGroup(const QString &group);

    // TODO: consider how the Renaming interface should work and map to Contacts
    // I guess it would be something like:
    // void renamedTo(Tp::ContactPtr)
    // with that contact getting the same features requested as the current one. Or would we rather
    // want to signal that change right away with a handle?

protected:
    // FIXME: (API/ABI break) Remove connectNotify
    void connectNotify(const char *);

private:
    Contact(ContactManager *manager, const ReferencedHandles &handle,
            const QSet<Feature> &requestedFeatures, const QVariantMap &attributes);

    void augment(const QSet<Feature> &requestedFeatures, const QVariantMap &attributes);

    void receiveAlias(const QString &alias);
    void receiveAvatarToken(const QString &avatarToken);
    void setAvatarToken(const QString &token);
    void receiveSimplePresence(const SimplePresence &presence);
    void receiveLocation(const QVariantMap &location);

    void setSubscriptionState(PresenceState state);
    void setPublishState(PresenceState state);
    void setBlocked(bool value);

    void setAddedToGroup(const QString &group);
    void setRemovedFromGroup(const QString &group);

    struct Private;
    friend class ContactManager;
    friend struct Private;
    Private *mPriv;
};

typedef QSet<ContactPtr> Contacts;

// FIXME: (API/ABI break) Remove once Contact is a SharedPtr and add a new qHash(SharedPtr<T>)
inline uint qHash(const ContactPtr &contact)
{
    return qHash(contact.data());
}

} // Tp

#endif
