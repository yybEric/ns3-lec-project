实验报告Project2
==
杨奕波
========
###* 实验结果及分析
###*Project2_1拓扑结构图<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic2/top1.png)<br>
###* Project2_1运行代码终端图<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic2/jieguo1.png)<br>
###*Project2_2拓扑结构图<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic2/top2.png)<br>
###* Project2_2运行代码终端图<br>
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic2/jieguo2.png)<br>
###* Project2_1中STA移动模型配置代码<br>
```cpp
//配置wifi移动模型，线性匀速度移动。
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("100.0"),
                                 "Y", StringValue ("100.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=30]"));
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", StringValue ("Time"),
                             "Time", StringValue ("100s"),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                             "Bounds", StringValue ("0|200|0|200"));
  mobility.Install (wifiStaNodes);
```
###* Project2_2中STA移动模型配置代码，另一种配置线性匀速度移动模型<br>
```cpp
  //配置移动模型，起始位置
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  //STA移动模型ns3::ConstantVelocityMobilityModel
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (wifiStaNodes);
  
  Ptr <ConstantVelocityMobilityModel> myConstantVelocityMobilityModel = wifiStaNodes.Get(0)->GetObject<ConstantVelocityMobilityModel>();
  myConstantVelocityMobilityModel->SetVelocity(Vector3D (10, 0, 0));
```
*ConstantVelocityMobilityModel.cc文件里，没有找到任何attribute比如Position或则Velocity,所以只有用SetVelocity方式设置Velocity。

