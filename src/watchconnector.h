/*
  Copyright (C) 2014 Jouni Roivas
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the authors nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef WATCHCONNECTOR_H
#define WATCHCONNECTOR_H

#include <QObject>
#include <QtQml>
#include <QStringList>
#include <QBluetoothDeviceInfo>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>

using namespace QtBluetooth;

namespace watch
{

class WatchConnector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString isConnected READ isConnected NOTIFY connectedChanged)
public:
    enum {
        watchTIME = 11,
        watchVERSION = 16,
        watchPHONE_VERSION = 17,
        watchSYSTEM_MESSAGE = 18,
        watchMUSIC_CONTROL = 32,
        watchPHONE_CONTROL = 33,
        watchAPPLICATION_MESSAGE = 48,
        watchLAUNCHER = 49,
        watchLOGS = 2000,
        watchPING = 2001,
        watchLOG_DUMP = 2002,
        watchRESET = 2003,
        watchAPP = 2004,
        watchAPP_LOGS = 2006,
        watchNOTIFICATION = 3000,
        watchRESOURCE = 4000,
        watchAPP_MANAGER = 6000,
        watchSCREENSHOT = 8000,
        watchPUTBYTES = 48879
    };
    enum {
        callANSWER = 1,
        callHANGUP = 2,
        callGET_STATE = 3,
        callINCOMING = 4,
        callOUTGOING = 5,
        callMISSED = 6,
        callRING = 7,
        callSTART = 8,
        callEND = 9
    };
    explicit WatchConnector(QObject *parent = 0);
    virtual ~WatchConnector();
    bool isConnected() const { return is_connected; }
    QString name() const { if (socket != nullptr) return socket->peerName(); return ""; }

    QString timeStamp();
    QString decodeEndpoint(unsigned int val);

signals:
    void messageReceived(QString peer, QString msg);
    void nameChanged();
    void connectedChanged();
    void hangup();

public slots:
    void sendData(const QByteArray &data);
    void sendMessage(unsigned int endpoint, QByteArray data);
    void ping(unsigned int val);
    void sendNotification(unsigned int lead, QString sender, QString data, QString subject);
    void sendSMSNotification(QString sender, QString data);
    void sendEmailNotification(QString sender, QString data, QString subject);

    void buildData(QByteArray &res, QStringList data);
    QByteArray buildMessageData(unsigned int lead, QStringList data);

    void phoneControl(char act, unsigned int cookie, QStringList datas);
    void ring(QString number, QString name, bool incoming=true, unsigned int cookie=0);
    void startPhoneCall(unsigned int cookie=0);
    void endPhoneCall(unsigned int cookie=0);

    void deviceConnect(const QString name, const QString address);
    void deviceDiscovered(const QBluetoothDeviceInfo&);
    void handleWatch(const QBluetoothDeviceInfo&);
    void readSocket();
    void connected();
    void disconnected();
    void reconnect();

private:
    void decodeMsg(QByteArray data);
    QBluetoothSocket *socket;
    bool is_connected;
    QString _last_name;
    QString _last_address;
};

void registerWatchConnector();

}

#endif // WATCHCONNECTOR_H
