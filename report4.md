实验学习报告Project3
==
杨奕波
========
```cpp
1、搭建拓扑：两个wifi网络，各有6个sta，1个ap；实现两个wifi网络之间的通信
	2、场景范围：500m*500m
	3、传播模型为：
		ConstantSpeedPropagationDelayModel	
	4、Mac类型为：NqosWifiMacHelper（没有QoS保障的Mac层机制）
	5、移动模型为随机游走模型，设定初始位置为两个六边形，边长10m，sta在6个顶点上，ap在六边形中间；两个六边形中心相距50m
	6、packet size为512byte，包间隔为100ms
	7、使用PyViz查看仿真并截图
	8、自行设定自变量，使用Tracing系统记录数据，通过Matlab处理数据并画图
```

###  实验拓扑
1.生成两个WIFI网络，各有6个STA 2.两个WIFI网络中个有一个AP 3.这两个通过第三个WIFI网络相连实现核心网络
### 传播模型设置<br>
* 设置传播模型为ConstantSpeedPropagationDelayModel
```cpp
YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
```cpp
###  Mac类型设置<br>
```cpp
NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();
```cpp
### 移动模型设置<br>
```cpp
  Ptr<ListPositionAllocator> positionAlloc_2 = CreateObject<ListPositionAllocator> ();
  positionAlloc_2->Add (Vector (55.0, 0.0, 0.0)); 
  positionAlloc_2->Add (Vector (50.0, 8.66, 0.0)); 
  positionAlloc_2->Add (Vector (55.0, 17.32, 0.0)); 
  positionAlloc_2->Add (Vector (65.0, 17.32, 0.0)); 
  positionAlloc_2->Add (Vector (70.0, 8.66, 0.0)); 
  positionAlloc_2->Add (Vector (65.0, 0.0, 0.0));
  positionAlloc_2->Add (Vector (60.0, 8.66, 0.0));
  mobility_2.SetPositionAllocator (positionAlloc_2);
  //配置STA移动方式，RandomWalk2dMobilityModel，随机游走模型
  mobility_2.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
	"Bounds", RectangleValue (Rectangle (0, 500, 0, 500)));            //范围500×500
  mobility_2.Install (wifiStaNodes_2);
```
```cpp
  MobilityHelper mobility;

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> (); 
  //第一个wifi网络的sta节点位置
  positionAlloc->Add (Vector (5.0, 0.0, 0.0)); 
  positionAlloc->Add (Vector (0.0, 8.66, 0.0)); 
  positionAlloc->Add (Vector (5.0, 17.32, 0.0)); 
  positionAlloc->Add (Vector (15.0, 17.32, 0.0)); 
  positionAlloc->Add (Vector (20.0, 8.66, 0.0)); 
  positionAlloc->Add (Vector (15.0, 0.0, 0.0));
////////////AP////////////////////
  positionAlloc->Add (Vector (10.0, 8.66, 0.0)); 
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
   "Mode", StringValue ("Time"), "Time", StringValue ("100s"),
   "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
      "Bounds", StringValue ("0|500|0|500"));
  mobility.Install (wifiStaNodes);
```
### echoAPP传输包参数设置
```cpp
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));
```
### 仿真结果<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic4/project4_1.png)<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic4/project4_2.png)<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic4/project4_3.png)<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic4/project4_4.png)<br>
