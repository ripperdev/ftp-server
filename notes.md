# 对题目的理解

## libpcap抓取ICMP/DNS的数据包
- ICMP
  - 频率较高 (正常的ping每秒最多只会发送两个数据包)
  - payload部分大小异常 (默认为64bit)
  - payload部分数据是否符合Windows/Linux标准包体
  - 响应数据包中payload跟请求包不一致
- DNS
  - 域名超长
  - 频率较高
  - 类型为TXT，有回包
  - 无A记录解析结果
  - 不存在全域名注册记录

