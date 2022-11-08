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
#include "ns3/network-server-helper.h"
#include "ns3/network-controller-components.h"
#include "ns3/adr-component.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("LoraSenderHelper");

LoraSenderHelper::LoraSenderHelper ()
{
  m_factory.SetTypeId ("ns3::NetworkServer");
}

LoraSenderHelper::~LoraSenderHelper ()
{
}

void
LoraSenderHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
LoraSenderHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
LoraSenderHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
LoraSenderHelper::InstallPriv (Ptr<Node> node) const
{
  NS_LOG_FUNCTION (this << node);

  Ptr<NetworkServer> app = m_factory.Create<NetworkServer> ();

  app->SetNode (node);
  node->AddApplication (app);

  // Link the NetworkServer to the NetDevices
  for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      Ptr<NetDevice> currentNetDevice = node->GetDevice (i);
        else if (currentNetDevice->GetObject<CsmaNetDevice> () != 0)
        {
          Ptr<CsmaNetDevice> csmaNetDevice =
            currentNetDevice->GetObject<CsmaNetDevice> ();
          app->SetCsmaNetDevice (csmaNetDevice);
          csmaNetDevice->SetReceiveCallback (MakeCallback
											(&NetworkServer::Send, app));
        }
      else
        {
          NS_LOG_ERROR ("Potential error: NetDevice is not CSMA");
        }
    }

  return app;
}
}
} // namespace ns3
