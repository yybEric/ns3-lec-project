实验学习报告Project3
==
杨奕波
========
### * Logging系统
在脚本中定义和启动记录组件：
```cpp
NS_LOG_COMPONENT_DEFINE(“...”)；
LogComponentEnable（“...”，LOG_LEVEL_...）;
```
通过设置NS_LOG环境变量，在不改变脚本和重新编译的情况下来增加日志的级别：
```cpp
$ export NS_LOG=UdpEchoClientApplication=level_all
```
![](https://github.com/yybEric/ns3-lec-project/blob/master/pic3/p3_1.png)<br>
### * Tracing系统<br>
* ASCII
```cp
AsciiTraceHelper ascii;       //创建一个ASCII trace对象
pointToPoint.EnalbeAsciiAll (ascii.CreateFileStream("first.tr"));    //包含两个方法调用。
```
* PCAP Tracing
```cp
pointToPoint.EnalbePcapAllll ("first");
```

![](https://github.com/yybEric/ns3-lec-project/blob/master/pic3/p3_2.png)<br>
### * PyViz系统<br>
NS3中的仿真可视化工具，调用如下命令行：
./waf –-run project3_2 --vis<br>

![](https://github.com/yybEric/ns3-lec-project/blob/master/pic3/p3_3.png)<br>

### * Wireshark工具使用<br>
如果使用Wireshark工具打开由Tracing系统记录的pcap文件，可以看到：

![](https://github.com/yybEric/ns3-lec-project/blob/master/pic3/p3_4.png)<br>
### * Matlab数据处理<br>
用Matlab计算发送与接收时间差：

![](https://github.com/yybEric/ns3-lec-project/blob/master/pic3/p3_6.jpg)<br>
### * Wireshark安装步骤<br>
```cpp
$sudo apt-get update
$sudo apt-get install wireshark
```
### * PyViz安装步骤<br>
```cpp
sudo apt-get install python-dev python-pygraphviz python-kiwi python-pygoocanvas python-gnome2 python-gnomedesktop python-rsvg
```
安装交互python
```cpp
$sudo apt-get install ipython
```
参考资料：http://blog.csdn.net/bo5509/article/details/8189757 <br>
http://blog.csdn.net/mars_nudt/article/details/7464088 <br>
