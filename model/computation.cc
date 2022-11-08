#include "ns3/computation.h"
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

NS_LOG_COMPONENT_DEFINE ("Computation");

NS_OBJECT_ENSURE_REGISTERED (Computation);

TypeId
Computation::GetTypeId (void) 
{
  static TypeId tid = TypeId ("ns3::Computation")
    .SetParent<Application> ()
    .AddConstructor<Computation> ()
    .SetGroupName ("lorawan")
    .AddAttribute("Shares","Minimum amount of shares to compute secret key.",
				   IntegerValue (0),
                   MakeIntegerAccessor (&Computation::m_shares),
                   MakeIntegerChecker<int64_t>());
  return tid;
}

Computation::Computation ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Computation::~Computation ()
{
  NS_LOG_FUNCTION_NOARGS ();
}


bool
Computation::SetCount (int count){
	m_count = count;
	return true;
	}

int
Computation::GetCount (){
	return m_count;
	}


void
Computation::SetCsmaNetDevice (Ptr<CsmaNetDevice> csmaNetDevice)
{
  NS_LOG_FUNCTION (this << csmaNetDevice);

  
  m_csmaNetDevice = csmaNetDevice;
}

void
Computation::SetLoraNetDevice (Ptr<LoraNetDevice> loraNetDevice)
{
  NS_LOG_FUNCTION (this << loraNetDevice);

  
  m_loraNetDevice = loraNetDevice;
}


bool
Computation::SendToCsma (Ptr<NetDevice> csmaNetDevice,
                                    Ptr<const Packet> packet, uint16_t protocol,
                                    const Address& sender)
{
  NS_LOG_FUNCTION (this << packet << protocol << sender);
	int count = GetCount();
  Ptr<Packet> packetCopy = packet->Copy ();
  
    //NS_LOG_UNCOND ("------------------------PACKET MADE IT TO CONTROL CENTER--------------------------------");
     SetCount(count+1);
  
  //NS_LOG_UNCOND(m_count);
  // if (GetCount()==m_shares){
	if (GetCount()==99999999){
	//NS_LOG_UNCOND("Packet is being sent from CC to NS");
	m_csmaNetDevice->Send (packetCopy,Mac48Address("02-06-00:00:00:00:00:01"),0x0800);
	
		}
	
  
  //0x800


  return true;
}

bool
Computation::SendToLora (Ptr<NetDevice> loraNetDevice,
                                    Ptr<const Packet> packet, uint16_t protocol,
                                    const Address& sender){
										  

	Ptr<Packet> packetCopy = packet->Copy ();
	
	
	//NS_LOG_UNCOND ("-----------------------PACKET SHOULD BE INSIDE END DEVICES------------------------------");
	
	return true;
	
	}

void
Computation::StartApplication (void)
{
  NS_LOG_FUNCTION (this);
  SetCount(0);

  // TODO Make sure we are connected to both needed devices
}

void
Computation::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // TODO Get rid of callbacks
}

}
}

