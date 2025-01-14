echo "configure terminal"
echo "interface eth0.1"
echo "no ip address 192.168.1.1/24"
echo "ip address 10.0.91.129/27"
echo "exit"
echo "interface eth0.2"
echo "no ip address 192.168.2.1/24"
echo "ip address 10.0.91.186/30"
echo "exit"
echo "interface eth0.4"
echo "no ip address 192.168.4.1/24"
echo "ip address 10.0.91.182/30"
echo "exit"
echo "ip route 10.0.91.176/30 10.0.91.181 1"
echo "ip route 10.0.91.176/30 10.0.91.185 2"
echo "ip route 10.0.91.184/30 eth0.2 1"
echo "ip route 10.0.91.184/30 10.0.91.181 2"
echo "ip route 10.0.91.0/25 10.0.91.185 1"
echo "ip route 10.0.91.0/25 10.0.91.181 2"
echo "ip route 10.0.91.160/28 10.0.91.181 1"
echo "ip route 10.0.91.160/28 10.0.91.185 2"
echo "ip route 10.0.91.180/30 eth0.4 1"
echo "ip route 10.0.91.180/30 10.0.91.185 2"
echo "ip route 10.0.91.128/27 eth0.1 1"
echo "exit"
echo "exit"
