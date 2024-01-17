/**
    QDeconz2MQTT publishes deconz events to a MQTT broker.
    Copyright (C) 2024  Thomas Zimmermann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MQTTSUBSCRIPTION_H
#define MQTTSUBSCRIPTION_H

#include <QObject>
#include <QMqttClient>
#include <QMqttSubscription>
#include <QUrl>
#include <QVariant>

#include "deconz2mqttconfig.h"

class MqttHandler : public QObject
{
    Q_OBJECT
public:
    MqttHandler(const Deconz2MQTTConfig & config, const QString &topic, QObject *parent = nullptr);

signals:
    void errorOccured(QMqttClient::ClientError error);

public slots:
    void handleMessage(const QVariant &msgContent);

private slots:
    void subscribe();
    void onConnectionError(QMqttClient::ClientError error);

private:
    QMqttClient m_client;
    QMqttSubscription *m_subscription;
    QString m_topic;
};
#endif // MQTTSUBSCRIPTION_H
