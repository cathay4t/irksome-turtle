# irksome-turtle
Personal used encrypted IPv4 in IPv4 tunnel.

The encryption code(`encyrpt.c`) will not be included due to security concern.

Usage:
```
    Local:
        sudo ./irksome -s <local_ip> -d <remote_ip>
        sudo ./tun_setup.sh

    Remote:
        sudo ./irksome -s <local_ip> -d <remote_ip>
        sudo ./tun_setup_peer.sh
```

Documents referred:
 * TUN based:
    http://backreference.org/2010/03/26/tuntap-interface-tutorial/
 * IP based, try to use one of these IP protocol ID:
    http://www.iana.org/assignments/protocol-numbers/protocol-numbers.txt
