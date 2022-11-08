#include "ns3/sender.h"
#include "ns3/log.h"
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

NS_LOG_COMPONENT_DEFINE ("Sender");

NS_OBJECT_ENSURE_REGISTERED (Sender);

TypeId
Sender::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Sender")
    .SetParent<Application> ()
    .AddConstructor<Sender> ()
    .SetGroupName ("lorawan");
  return tid;
}

Sender::Sender ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Sender::~Sender ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Sender::SetPointToPointNetDevice (Ptr<PointToPointNetDevice>
                                     pointToPointNetDevice)
{
  NS_LOG_FUNCTION (this << pointToPointNetDevice);

  m_pointToPointNetDevice = pointToPointNetDevice;
}

void
Sender::SetCsmaNetDevice (Ptr<CsmaNetDevice> csmaNetDevice)
{
  NS_LOG_FUNCTION (this << csmaNetDevice);

  m_csmaNetDevice = csmaNetDevice;
}

bool
Sender::SendToPointToPoint (Ptr<NetDevice> csmaNetDevice, Ptr<const Packet>
                            packet, uint16_t protocol, const Address& sender)
{
  NS_LOG_FUNCTION (this << packet << protocol << sender);
	///Send to GW
  
  Ptr<Packet> packetCopy = packet->Copy ();
 //HERE
  
  
  //Simulator::Schedule(Seconds (20.0), &NetworkServer::Send,this, packetCopy, LoraDeviceAddress(0xFF, 0xFFFFFFFF));
  
  //NS_LOG_UNCOND("Packet = "<<LoraDeviceAddress(0xFF, 0xFFFFFFFF));
  //m_networkServer->Send (packetCopy,LoraDeviceAddress(0xFF, 0xFFFFFFFF));
  
  //NS_LOG_UNCOND("Network Server is sending packet to GW");
  m_pointToPointNetDevice->Send (packetCopy,
                                 m_pointToPointNetDevice->GetBroadcast (),
                                 0x800);
  return true;
 
}

bool
Sender::SendToCsma (Ptr<NetDevice> pointToPointNetDevice,
                                    Ptr<const Packet> packet, uint16_t protocol,
                                    const Address& sender)
{
  NS_LOG_FUNCTION (this << packet << protocol << sender);

  Ptr<Packet> packetCopy = packet->Copy ();
  packetCopy->RemoveAllPacketTags();
  ///Send To CC
  m_csmaNetDevice->Send (packetCopy,Mac48Address("02-06-00:00:00:00:00:02"),0x0800);
  //0x800


  return true;
}

void
Sender::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  // TODO Make sure we are connected to both needed devices
}

void
Sender::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // TODO Get rid of callbacks
}

}
}
