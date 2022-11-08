#include "ns3/point-to-point-module.h"
#include "ns3/forwarder-helper.h"
#include "ns3/sender-helper.h"
#include "ns3/computation-helper.h"
#include "ns3/network-server-helper.h"
#include "ns3/lora-channel.h"
#include "ns3/mobility-helper.h"
#include "ns3/lora-phy-helper.h"
#include "ns3/lorawan-mac-helper.h"
#include "ns3/lora-helper.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/periodic-sender.h"
#include "ns3/periodic-sender-helper.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/command-line.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/lora-device-address-generator.h"
#include "ns3/one-shot-sender-helper.h"
#include "ns3/command-line.h"
#include "ns3/netanim-module.h"
#include <iostream>
#include <string>
#include "ns3/correlated-shadowing-propagation-loss-model.h"
#include "ns3/building-penetration-loss.h"
#include "ns3/building-allocator.h"
#include "ns3/buildings-helper.h"

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE ("LoraCenter");

void
SendPacket (Ptr<NetworkServer> server, Ptr<Packet> pkt, LoraDeviceAddress endDevice, int option)
{
  //NS_LOG_UNCOND("Sent Time: ");
  NS_LOG_UNCOND("Pub "<<Simulator::Now().GetMilliSeconds());
  if (!option)
    {
      //Broadcast
      server->SendBroad (pkt);
    }
  else
    {
      //Unicast
      server->SendUni (pkt, endDevice);
    }
}

int
main (int argc, char *argv[])
{
  //VARIABLES

  int nED; ///Number of End Devices
  int tTime = 0; ///Total Time
  int nGW;
  double intervalED; ///Amount of Time waited between each endDevice
  double intervalNS;

  int cDelay;
  int option;
  int seedVal = 1;
  int radius;

  //LOGGING

  if (false)
    {
      //LogComponentEnable ("LoraCenter", LOG_LEVEL_ALL);
      //LogComponentEnable ("LoraPacketTracker",RemoveHeader LOG_LEVEL_ALL);
      //LogComponentEnable ("GatewayLorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);
      //LogComponentEnable("LorawanMacHeader", LOG_LEVEL_ALL);
      //LogComponentEnable("MacCommand", LOG_LEVEL_ALL);
      //LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraChannel", LOG_LEVEL_ALL);
      //LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
      //LogComponentEnable ("EndDeviceLorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable ("ClassCEndDeviceLorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable ("OneShotSender", LOG_LEVEL_ALL);
      //LogComponentEnable("PointToPointNetDevice", LOG_LEVEL_ALL);
      //LogComponentEnable ("OneShotSender", LOG_LEVEL_ALL);
      //LogComponentEnable ("DeviceStatus", LOG_LEVEL_ALL);
      //LogComponentEnable ("GatewayStatus", LOG_LEVEL_ALL);
      //LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("LorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable("EndDeviceLorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
      //LogComponentEnable("LogicalLoraChannel", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraHelper", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraPhyHelper", LOG_LEVEL_ALL);
      //LogComponentEnable("LorawanMacHelper", LOG_LEVEL_ALL);
      //LogComponentEnable("PeriodicSenderHelper", LOG_LEVEL_ALL);
      //LogComponentEnable("PeriodicSender", LOG_LEVEL_ALL);
      //LogComponentEnable("LorawanMacHeader", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);
      //LogComponentEnable("NetworkScheduler", LOG_LEVEL_ALL);
      LogComponentEnable ("NetworkServer", LOG_LEVEL_ALL);
      //LogComponentEnable("NetworkStatus", LOG_LEVEL_ALL);
      //LogComponentEnable("NetworkController", LOG_LEVEL_ALL);

      //I added this Yacoub
      //LogComponentEnable ("Sender", LOG_LEVEL_ALL);
      //LogComponentEnable ("Forwarder", LOG_LEVEL_ALL);
      ////LogComponentEnable("LoraChannel", LOG_LEVEL_INFO);
      ////LogComponentEnable("LoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraInterferenceHelper", LOG_LEVEL_ALL);
      LogComponentEnable ("ClassCEndDeviceLorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable("GatewayLorawanMac", LOG_LEVEL_ALL);
      //LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
      //LogComponentEnable("LoraNetDevice", LOG_LEVEL_ALL);
      LogComponentEnable ("LoraPacketTracker", LOG_LEVEL_ALL);

      LogComponentEnableAll (LOG_PREFIX_FUNC);
      LogComponentEnableAll (LOG_PREFIX_NODE);
      LogComponentEnableAll (LOG_PREFIX_TIME);
    }

  //PARSECOMMANDLINE

  CommandLine cmd;
  cmd.AddValue ("endDevices", "Amount of EndDevices for simulation", nED);
  cmd.AddValue ("time", "Duration of simulation", tTime);
  cmd.AddValue ("gateway", "Number of Gateways", nGW);
  cmd.AddValue ("intervalED", "Interval between shares", intervalED);
  cmd.AddValue ("intervalNS", "Interval between shares", intervalNS);
  cmd.AddValue ("computationDelay", "Expected Computation Delay", cDelay);
  cmd.AddValue ("transmission", "0 -> Broadcast; 1-> Unicast", option);
  cmd.AddValue ("seed", "Change random variable seed.", seedVal);
  cmd.AddValue ("radius", "Change radius.", radius);
  cmd.Parse (argc, argv);

  RngSeedManager::SetSeed (seedVal);
  Config::SetDefault ("ns3::Computation::Shares", IntegerValue (nED));
  Config::SetDefault ("ns3::NetworkServer::Delay", DoubleValue (cDelay));
  Config::SetDefault ("ns3::NetworkServer::Interval", DoubleValue (intervalNS));
  Config::SetDefault ("ns3::NetworkServer::Option", IntegerValue (option));

  if (tTime == 0)
    {
      //Assume you want to calculate it
      tTime = ((nED * intervalED)*(nED * intervalED)) + 200;
    }

  //NODECONTAINERS

  ///ENDDEVICES

  NodeContainer endDevices;
  endDevices.Create (nED);

  ///GATEWAY

  NodeContainer gateway;
  gateway.Create (nGW);

  ///NETWORKSERVER

  NodeContainer networkServer;
  networkServer.Create (1);

  ///CONTROLCENTER

  NodeContainer controlCenter;
  controlCenter.Create (1);

  ///CSMACONTAINER
  NodeContainer csmaNodes;
  csmaNodes.Add (networkServer);
  csmaNodes.Add (controlCenter);

  //Channel

  ///LORA

  Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel> ();
  loss->SetPathLossExponent (3.76);
  loss->SetReference (1, 7.7);

  if (false)
    {
      // Create the correlated shadowing component
      Ptr<CorrelatedShadowingPropagationLossModel> shadowing =
          CreateObject<CorrelatedShadowingPropagationLossModel> ();

      // Aggregate shadowing to the logdistance loss
      loss->SetNext (shadowing);
    }

  Ptr<PropagationDelayModel> delay = CreateObject<RandomPropagationDelayModel> ();
  Ptr<LoraChannel> channel = CreateObject<LoraChannel> (loss, delay);

  ///CSMA

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Llc"));

  //Mobility
  ///End Device mobility

  //ObjectFactory pos;
  //pos.SetTypeId ("ns3::RandomRectanglePositionAllocator");
  //pos.Set ("X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1000]"));
  //pos.Set ("Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1000]"));
  //Ptr<PositionAllocator> positionAllocEd = pos.Create ()->GetObject<PositionAllocator> ();

  MobilityHelper mobilityEd;
  //mobilityEd.SetPositionAllocator (positionAllocEd);
  mobilityEd.SetPositionAllocator ("ns3::UniformDiscPositionAllocator", "X", DoubleValue (250), "Y",
                                   DoubleValue (250), "Z", DoubleValue (0), "rho",
                                   DoubleValue (radius));
  mobilityEd.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityEd.Install (endDevices);

  ///Gateway mobility

  MobilityHelper mobilityGw;
  Ptr<ListPositionAllocator> positionAllocGw = CreateObject<ListPositionAllocator> ();
  for (int i = 0; i < nGW; i++)
    {
      positionAllocGw->Add (Vector (250.0, 250, 15));
    }
  mobilityGw.SetPositionAllocator (positionAllocGw);
  mobilityGw.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityGw.Install (gateway);

  ///NetworkServer mobility

  MobilityHelper mobilityNs;
  Ptr<ListPositionAllocator> positionAllocNs = CreateObject<ListPositionAllocator> ();
  positionAllocNs->Add (Vector (250.0, 125, 15));
  //positionAllocGw->Add (Vector (-2000.0, 0.0, 0.0));		Use this for multiple gateway locations
  mobilityNs.SetPositionAllocator (positionAllocNs);
  mobilityNs.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityNs.Install (networkServer);

  ///ControlCenter mobility

  MobilityHelper mobilityCc;
  Ptr<ListPositionAllocator> positionAllocCc = CreateObject<ListPositionAllocator> ();
  positionAllocCc->Add (Vector (250.0, 0, 15));
  //positionAllocGw->Add (Vector (-2000.0, 0.0, 0.0));		Use this for multiple gateway locations
  mobilityCc.SetPositionAllocator (positionAllocCc);
  mobilityCc.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityCc.Install (controlCenter);

  //Helpers

  ///Lora
  LoraPhyHelper phyHelper = LoraPhyHelper ();
  phyHelper.SetChannel (channel);
  LorawanMacHelper macHelper = LorawanMacHelper ();
  LoraHelper helper = LoraHelper ();
  helper.EnablePacketTracking ();
  //Create a LoraDeviceAddressGenerator
  uint8_t nwkId = 54;
  uint32_t nwkAddr = 1864;
  Ptr<LoraDeviceAddressGenerator> addrGen =
      CreateObject<LoraDeviceAddressGenerator> (nwkId, nwkAddr);

  //NetDevices

  ///Lora
  //EndDevices
  phyHelper.SetDeviceType (LoraPhyHelper::ED);
  macHelper.SetDeviceType (LorawanMacHelper::ED_C);
  macHelper.SetAddressGenerator (addrGen);
  macHelper.SetRegion (LorawanMacHelper::EU);
  helper.Install (phyHelper, macHelper, endDevices);

  Ptr<LorawanMac> edMac[nED];
  Ptr<ClassCEndDeviceLorawanMac> edLorawanMac[nED];

  for (int i = 0; i < nED; i++)
    {
      edMac[i] = endDevices.Get (i)->GetDevice (0)->GetObject<LoraNetDevice> ()->GetMac ();
      edLorawanMac[i] = edMac[i]->GetObject<ClassCEndDeviceLorawanMac> ();
    }

  //Ptr<LorawanMac> edMac0 = endDevices.Get (0)->GetDevice (0)->GetObject<LoraNetDevice> ()->GetMac ();
  //Ptr<ClassCEndDeviceLorawanMac> edLorawanMac0 = edMac0->GetObject<ClassCEndDeviceLorawanMac> ();
  //Ptr<LorawanMac> edMac1 = endDevices.Get (1)->GetDevice (0)->GetObject<LoraNetDevice> ()->GetMac ();
  //Ptr<ClassCEndDeviceLorawanMac> edLorawanMac1 = edMac1->GetObject<ClassCEndDeviceLorawanMac> ();

  //Gateway
  phyHelper.SetDeviceType (LoraPhyHelper::GW);
  macHelper.SetDeviceType (LorawanMacHelper::GW);
  helper.Install (phyHelper, macHelper, gateway);

  // Set spreading factors up
  macHelper.SetSpreadingFactorsUp (endDevices, gateway, channel);

  ///CSMA
  NetDeviceContainer csmaNetDevs = csma.Install (csmaNodes);
  InternetStackHelper ipStack;
  ipStack.Install (csmaNodes);

  // assign ip addresses
  NS_LOG_INFO ("Assign ip addresses.");
  Ipv4AddressHelper ip;
  ip.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer addresses = ip.Assign (csmaNetDevs);
  //NS_LOG_UNCOND(addresses.GetAddress(0));
  //NS_LOG_UNCOND(csmaNetDevs.Get(1)->GetAddress());
  //NS_LOG_UNCOND(edLorawanMac0->Print());

  //Applications

  ///EndDevices

  //PeriodicSenderHelper appHelper = PeriodicSenderHelper ();
  //appHelper.SetPeriod (Seconds (81));
  //appHelper.SetPacketSize (10);
  //ApplicationContainer appContainer = appHelper.Install (endDevices);

  //appContainer.Start (Seconds (0));
  //appContainer.Stop (Seconds(80));

  OneShotSenderHelper oneShotHelper = OneShotSenderHelper ();
  // oneShotHelper.SetSendTime (Seconds (5));
  // oneShotHelper.Install (endDevices.Get (0));

  for (int i = 0; i < nED; i++)
    {
      oneShotHelper.SetSendTime (Seconds ((i * intervalED)));
      oneShotHelper.Install (endDevices.Get (i));
    }
  int ttWait = (nED*intervalED);
  //NS_LOG_UNCOND("ttWait is:");
  //NS_LOG_UNCOND(ttWait);
  //OneShotSenderHelper oneShotHelper = OneShotSenderHelper ();
  //oneShotHelper.SetSendTime (Seconds (4));
  //oneShotHelper.Install (endDevices);

  //oneShotHelper.Install (endDevices.Get(1));

  ///NetworkServer
  NetworkServerHelper networkServerHelper;
  networkServerHelper.SetGateways (gateway);
  networkServerHelper.SetEndDevices (endDevices);
  ApplicationContainer apps = networkServerHelper.Install (networkServer);
  Ptr<NetworkServer> ns = DynamicCast<NetworkServer> (apps.Get (0));

  ///Gateway
  ForwarderHelper forwarderHelper;
  forwarderHelper.Install (gateway);

  SenderHelper sender;
  sender.Install (networkServer);

  ComputationHelper computation;
  // NodeContainer computaters;
  //computaters.Add(controlCenter);
  //computaters.Add(endDevices);
  computation.Install (controlCenter);

  // First Phase Unicast
  //for (int i = 0; i < nED; i++)
  //  {
  //    
  //    Simulator::Schedule (Seconds (ttWait + (i * intervalED)), &SendPacket, ns, Create<Packet> (39),
  //                         edLorawanMac[i]->GetDeviceAddress (), 1);
  //  } 
  ttWait = ttWait + (nED * intervalED);
  //NS_LOG_UNCOND(ttWait);
  // Second Phase Broadcast
  Simulator::Schedule (Seconds (ttWait), &SendPacket, ns, Create<Packet> (39),
                       LoraDeviceAddress (0xFF, 0xFFFFFFFF), 0);

  // // Try to send a unicast packet to the end-device
  // if (!option)
  //   {
  //     Simulator::Schedule (Seconds (50), &SendPacket, ns, Create<Packet> (20),
  //                          LoraDeviceAddress (0xFF, 0xFFFFFFFF), 0);
  //   }
  // else
  //   {
  //     for (int i = 0; i < nED; i++)
  //       {
  //         Simulator::Schedule (Seconds (50 + (i * 25)), &SendPacket, ns, Create<Packet> (20),
  //                              edLorawanMac[i]->GetDeviceAddress (), 1);
  //       }
  //   }

  //Simulator::Schedule(Seconds (30.0), &SendPacket, ns, Create<Packet>(10), edLorawanMac[1]->GetDeviceAddress());
  //Simulator::Schedule(Seconds (26.0), &SendPacket, ns, Create<Packet>(10), edLorawanMac[1]->GetDeviceAddress());
  // Try to send a broadcast packet to the end-devices
  //Simulator::Schedule(Seconds (900.0), &SendPacket, ns, Create<Packet>(20), LoraDeviceAddress(0xFF, 0xFFFFFFFF));

  //PointToPointHelper p2ph;
  //p2ph.EnablePcapAll("P2P",true);

  csma.EnablePcap ("controlCenterCsma", csmaNetDevs.Get (1), true, true);
  csma.EnablePcap ("NetworkServerCsma", csmaNetDevs.Get (0), true, true);

  AnimationInterface anim ("test.xml");

  for (int i = 0; i < nED; i++)
    {
      std::string deviceName = "EndDevice#";
      deviceName = deviceName + std::to_string (i);
      anim.UpdateNodeDescription (endDevices.Get (i), deviceName);
    }

  anim.UpdateNodeDescription (networkServer.Get (0), "NetworkServer");
  anim.UpdateNodeDescription (gateway.Get (0), "GW");
  anim.UpdateNodeDescription (controlCenter.Get (0), "ControlCenter");

  // Start simulation
  Simulator::Stop (Seconds (tTime));
  Simulator::Run ();
  Simulator::Destroy ();

  LoraPacketTracker &tracker = helper.GetPacketTracker ();
  std::cout << tracker.CountMacPacketsGlobally (Seconds (0), Seconds (tTime)) << std::endl;

  return 0;
}
