/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

// Default Network Topology
//默认网络拓扑
// Number of wifi or csma nodes can be increased up to 250

//n4是EchoServer 其他节点是Cilent 
//   Wifi 10.1.3.0
//                      AP
//  *      *      *     *
//  |      |      |     |      10.1.1.0				    10.1.4.0
// n5     n6     n7    n0 ------------------ n1   n2   n3   n4------------------n8   n9   n10  n11
//                          point-to-point   |    |    |    |    point-to-point  |    |    |    | 
//                                           ================			 ================
//                                              LAN 10.1.2.0			   LAN 10.1.5.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");		//定义记录组件

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 3;			//csma节点数量
  uint32_t nWifi = 3;				//wifi节点数量
   bool tracing = true;


  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue		//判断是否超过了250个，超过报错 , 原因？
  if (nWifi > 250 || nCsma > 250)
    {
      std::cout << "Too many wifi or csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//启动记录组件
    }


  //创建2个节点，p2p链路两端
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  //创建信道，设置信道参数，在设备安装到节点上
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);
  
  //创建信道2，设置信道参数，在设备安装到节点上
  NodeContainer p2pNodes_2;
  p2pNodes_2.Create (2);

  NetDeviceContainer p2pDevices_2;
  p2pDevices_2 = pointToPoint.Install (p2pNodes_2);

  //创建csma 1节点，包含两个个p2p节点

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma-1);
  csmaNodes.Add (p2pNodes_2.Get (0));

  //创建信道，设置信道参数，在设备安装到节点上
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  //创建csma 2节点，包含一个p2p节点
  NodeContainer csmaNodes_2;
  csmaNodes_2.Add (p2pNodes_2.Get (1));
  csmaNodes_2.Create (nCsma);

  CsmaHelper csma_2;
  csma_2.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma_2.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  //创建信道2，设置信道参数，在设备安装到节点上
  NetDeviceContainer csmaDevices_2;
  csmaDevices_2 = csma_2.Install (csmaNodes_2);

//创建wifista无线终端，AP接入点
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = p2pNodes.Get (0);

  //创建无线设备于无线节点之间的互联通道，并将通道对象与物理层对象关联
  //确保所有物理层对象使用相同的底层信道，即无线信道
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

  //配置mac类型为sta模式，不发送探测请求
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  //创建无线设备，将mac层和phy层安装到设备上
  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  //配置AP节点的mac层为AP模式，创建AP设备
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  //配置移动模型，起始位置
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

///////////////////////////////////////STA移动模型ns3::ConstantVelocityMobilityModel
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (wifiStaNodes);
  
  Ptr <ConstantVelocityMobilityModel> myConstantVelocityMobilityModel = wifiStaNodes.Get(0)->GetObject<ConstantVelocityMobilityModel>();
  myConstantVelocityMobilityModel->SetVelocity(Vector3D (10, 0, 0));

//配置AP移动方式，ConstantPositionMobilityModel，固定位置模型
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

  //已经创建了节点，设备，信道和移动模型，接下来配置协议栈
  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (csmaNodes_2);
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  //分配IP地址
  Ipv4AddressHelper address;
 //P2P信道
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);
 //csma 1信道
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);
 //wifi信道
  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);
 //P2P 2信道
  address.SetBase ("10.1.4.0", "255.255.255.0");
  address.Assign (p2pDevices_2);
 //csma 2信道
  address.SetBase ("10.1.5.0", "255.255.255.0");
  address.Assign (csmaDevices_2);

  //放置echo服务端程序在最右边的csma节点,端口为9
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma-1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma-1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  //客户端1通信Echoudp通信
  ApplicationContainer clientApps = 
  echoClient.Install (wifiStaNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));


 //客户端2通信Echoudp通信
  ApplicationContainer clientApps_2 = 
  echoClient.Install (csmaNodes_2.Get (nWifi - 2));
  clientApps_2.Start (Seconds (3.0));
  clientApps_2.Stop (Seconds (10.0));
/* 
 //客户端3通信Echoudp通信
  ApplicationContainer clientApps_3 = 
  echoClient.Install (csmaNodes_2.Get (nWifi - 1));
  clientApps_3.Start (Seconds (4.0));
  clientApps_3.Stop (Seconds (10.0));

 //客户端3通信Echoudp通信
  ApplicationContainer clientApps_4 = 
  echoClient.Install (csmaNodes.Get (nWifi - 1));
  clientApps_4.Start (Seconds (5.0));
  clientApps_4.Stop (Seconds (10.0));
*/
  //启动互联网络路由
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  if (tracing == true)
    {
      pointToPoint.EnablePcapAll ("third");
      phy.EnablePcap ("third", apDevices.Get (0));
      csma.EnablePcap ("third", csmaDevices.Get (0), true);
      csma.EnablePcap ("third", csmaDevices_2.Get (0), true);
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
