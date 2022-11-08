#ifndef COMPUTATION_H
#define COMPUTATION_H

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

namespace ns3 {
namespace lorawan {

/**
 * This application forwards packets between NetDevices:
 * LoraNetDevice -> PointToPointNetDevice and vice versa.
 */
class Computation : public Application
{
public:
  Computation ();
  ~Computation ();

  static TypeId GetTypeId (void);

  /**
   * Sets the device to use to communicate with the EDs.
   *
   * \param loraNetDevice The LoraNetDevice on this node.
   */
  void SetCsmaNetDevice (Ptr<CsmaNetDevice> csmaNetDevice);


  /**
   * Receive a packet from the PointToPointNetDevice
   */
  bool SendToCsma (Ptr<NetDevice> csmaNetDevice,
                                Ptr<const Packet> packet, uint16_t protocol,
                                const Address& sender);
                                
  void SetLoraNetDevice (Ptr<LoraNetDevice> loraNetDevice);
  
  bool SendToLora (Ptr<NetDevice> loraNetDevice,
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
  

  bool SetCount (int count);
  
  int GetCount (void) ;
  

private:
  Ptr<CsmaNetDevice> m_csmaNetDevice; //!< Pointer to the node's CsmaNetDevice
  Ptr<LoraNetDevice> m_loraNetDevice; //!< Pointer to the node's CsmaNetDevice
  int m_count;
  int m_shares;
 

};

} //namespace ns3

}
#endif /* FORWARDER */


