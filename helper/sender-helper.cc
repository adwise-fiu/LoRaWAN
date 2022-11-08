#include "ns3/sender-helper.h"
#include "ns3/random-variable-stream.h"
#include "ns3/sender.h"
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

NS_LOG_COMPONENT_DEFINE ("SenderHelper");

SenderHelper::SenderHelper ()
{
  m_factory.SetTypeId ("ns3::Sender");
}

SenderHelper::~SenderHelper ()
{
}

void
SenderHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
SenderHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
SenderHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
SenderHelper::InstallPriv (Ptr<Node> node) const
{
  NS_LOG_FUNCTION (this << node);

  Ptr<Sender> app = m_factory.Create<Sender> ();
  Ptr<NetworkServer> networkServer = node->GetApplication(0)->GetObject<NetworkServer> ();


  app->SetNode (node);
  node->AddApplication (app);

  // Link the Sender to the NetDevices
  for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      Ptr<NetDevice> currentNetDevice = node->GetDevice (i);
      if (currentNetDevice->GetObject<PointToPointNetDevice> () != 0)
        {
		  Ptr<PointToPointNetDevice> pointToPointNetDevice =
            currentNetDevice->GetObject<PointToPointNetDevice> ();

          app->SetPointToPointNetDevice (pointToPointNetDevice);
			//NS_LOG_UNCOND(node->GetNDevices ());
          pointToPointNetDevice->SetReceiveCallback (MakeCallback
                                                  (&Sender::SendToCsma,app));
        }
        else if (currentNetDevice->GetObject<CsmaNetDevice> () != 0)
        {
          Ptr<CsmaNetDevice> csmaNetDevice =
            currentNetDevice->GetObject<CsmaNetDevice> ();
          app->SetCsmaNetDevice (csmaNetDevice);
          //csmaNetDevice->SetReceiveCallback (MakeCallback(&NetworkServer::Receive,networkServer));
        }
       
      else
        {
          NS_LOG_ERROR ("Potential error: NetDevice is not PointToPoint");
        }
    }

  return app;
}
}
} // namespace ns3
