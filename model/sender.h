#ifndef SENDER_H
#define SENDER_H

#include "ns3/application.h"
#include "ns3/lora-net-device.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-server.h"

namespace ns3 {
namespace lorawan {

/**
 * This application forwards packets between NetDevices:
 * LoraNetDevice -> PointToPointNetDevice and vice versa.
 */
class Sender : public Application
{
public:
  Sender ();
  ~Sender ();

  static TypeId GetTypeId (void);

  /**
   * Sets the device to use to communicate with the EDs.
   *
   * \param loraNetDevice The LoraNetDevice on this node.
   */
  void SetCsmaNetDevice (Ptr<CsmaNetDevice> csmaNetDevice);

  /**
   * Sets the P2P device to use to communicate with the NS.
   *
   * \param pointToPointNetDevice The P2PNetDevice on this node.
   */
  void SetPointToPointNetDevice (Ptr<PointToPointNetDevice> pointToPointNetDevice);

  /**
   * Receive a packet from the LoraNetDevice.
   *
   * \param loraNetDevice The LoraNetDevice we received the packet from.
   * \param packet The packet we received.
   * \param protocol The protocol number associated to this packet.
   * \param sender The address of the sender.
   * \returns True if we can handle the packet, false otherwise.
   */
  bool SendToPointToPoint (Ptr<NetDevice> csmaNetDevice, Ptr<const Packet> packet,
                        uint16_t protocol, const Address& sender);

  /**
   * Receive a packet from the PointToPointNetDevice
   */
  bool SendToCsma (Ptr<NetDevice> pointToPointNetDevice,
                                Ptr<const Packet> packet, uint16_t protocol,
                                const Address& sender);

  /**
   * Start the application
   */
  void StartApplication (void);

  /**
   * Stop the application
   */
  void StopApplication (void);

private:
  Ptr<CsmaNetDevice> m_csmaNetDevice; //!< Pointer to the node's CsmaNetDevice

  Ptr<PointToPointNetDevice> m_pointToPointNetDevice; //!< Pointer to the
  Ptr<NetworkServer> m_networkServer;
  //!P2PNetDevice we use to
  //!communicate with the NS
};

} //namespace ns3

}
#endif /* FORWARDER */

