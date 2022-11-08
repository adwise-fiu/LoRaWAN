#include "ns3/computation-helper.h"
#include "ns3/random-variable-stream.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
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

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("ComputationHelper");

ComputationHelper::ComputationHelper ()
{
  m_factory.SetTypeId ("ns3::Computation");
}

ComputationHelper::~ComputationHelper ()
{
}

void
ComputationHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
ComputationHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
ComputationHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
ComputationHelper::InstallPriv (Ptr<Node> node) const
{
  NS_LOG_FUNCTION (this << node);

  Ptr<Computation> app = m_factory.Create<Computation> ();

  app->SetNode (node);
  node->AddApplication (app);


  // Link the Computation to the NetDevices
  for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      Ptr<NetDevice> currentNetDevice = node->GetDevice (i);
        if (currentNetDevice->GetObject<CsmaNetDevice> () != 0)
        {
          Ptr<CsmaNetDevice> csmaNetDevice =
            currentNetDevice->GetObject<CsmaNetDevice> ();
          app->SetCsmaNetDevice (csmaNetDevice);
          csmaNetDevice->SetReceiveCallback (MakeCallback(&Computation::SendToCsma,app));
          
        } else if(currentNetDevice->GetObject<LoraNetDevice> () != 0){
		//NS_LOG_UNCOND("SHOULDNT BE INSIDE HERE NEVER WHAT IS HAPPENING");
		Ptr<LoraNetDevice> loraNetDevice =
            currentNetDevice->GetObject<LoraNetDevice> ();
          app->SetLoraNetDevice (loraNetDevice);
          loraNetDevice->SetReceiveCallback (MakeCallback
                                               (&Computation::SendToLora, app));
			}
       
      else
        {
          NS_LOG_ERROR ("Potential error: No NetDevice on ControlCenter");
        }
    }

  return app;
}
}
} // namespace ns3

