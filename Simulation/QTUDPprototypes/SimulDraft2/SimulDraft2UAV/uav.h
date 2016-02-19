#ifndef UAV_H
#define UAV_H

#include <QObject>
#include <QUdpSocket>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QTextStream>
#include <QTimer>

#include "packet.h"
#include "ackpacket.h"
#include "actionpacket.h"
#include "infopacket.h"
#include "telemetrypacket.h"
//#include "messagebox.h"

#include <queue>
#include <vector>

/*
 * Protocol for sending packets.
 *
 * Startup
 * ----------------------------------------------------------------
 * 1) UAV Starts up.
 * 2) UAV receives Action(request info) signal from GS.
 * 3) UAV sends info packet to GS.
 * 4) UAV sends telemetry packet every 200ms until shutdown
 *
 * Rest of the time 
 * ----------------------------------------------------------------
 * - UAV must send an ACK packet to every received packet from GS.
 * - UAV sends waypoint packet when waypoint is reached
 * - As soon as a Action(Stop) is received
 *      - Stop every task.
 *      - UAV goes home.
 * - If Action(Shutdown) received
 *      - UAV does not do anything upon receiving any other packets.
 */

class UAV : public QObject
{
    Q_OBJECT
public:

    /* Alvin Truong added on 16-1-27*/
    const static int PACKET_LENGTH = 1000;
    const static int GS_PORT_NUM = 27015;
    const static int UAV_PORT_NUM = 27020;
    static int NUM_RECV_PACKETS;
    UAV(QWidget *parent = 0);

    /**
     * \brief   Member function takes a vector of packets and sends them all to the uav through UDP.
     * \param   packets represents a vector of action packets.
     *
     * \author  Alvin Truong
     * \date    16-1-27
     *
     **/
    void sendAllActionPackets(std::vector<Protocol::Packet*> packets);
    void sendAllActionPackets(std::queue<Protocol::Packet*> packets);
    /**
     * \brief   Function takes a single packet or a child of packet and sends it through UDP.
     * \param   packet represents a single packet or child of packet.
     *
     * \author  Alvin Truong
     * \date    16-1-27
     *
     **/
    void sendAPacket(Protocol::Packet* packet);

    /* Created Functions to print out information about packets
     * parameters(Packet type packet)
     * return void
     * created by: Daniel Ortega
     *
     **/
    void print_ack_packet(Protocol::AckPacket& packet);
    void print_action_packet(Protocol::ActionPacket& packet);
    void print_info_packet(Protocol::InfoPacket& packet);
    void print_telemetry_packet(Protocol::TelemetryPacket& packet);

private:
    /*
     * \brief   Determines what response should be given depending on packet received from GS. 
     * \param   ap is of type action packet.
     *
     * \author  Alvin Truong
     * \date    16-2-19
     */
    void respond_to_action_packet(Protocol::ActionPacket ap);

    private:
    // True when uav needs to keep sending Telemetry packets every 200ms until shutdown.
    bool receivedInfoPacketReq, uavWaypointsReady, uavFlying, stopAction, shutdownAction,
         uavOn;

    // Hard coded constants for uav simulation
    int battery, pointsStorable;
    int telemSeqNumber;

    // Timer needed for telemetry every 200ms
    QTimer *timer;

private slots:
    void processPendingDatagrams();

    /*
     * \brief   Generate a telemetry packet to simulate actual scenario of
     *          uav sending telemetry packet ever 200ms.
     * \param   None 
     *
     * \author  Alvin Truong
     * \date    20160219
     */
    void sendCurrentTelem();

private:
    QUdpSocket sendUdpSocket;
    QUdpSocket recvUdpSocket;
signals:

public slots:
};

#endif // UAV_H
