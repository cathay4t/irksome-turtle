/*
 * Copyright (C) 2015 Gris Ge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gris Ge <cnfourt@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define _TUN_NAME "turtle"

#define error(format, ...) \
	fprintf(stderr, "ERROR(%s:%d): " format, \
		__FILE__, __LINE__, ##__VA_ARGS__)

#define info(format, ...) \
	fprintf(stdout, "INFO(%s:%d): " format, \
		__FILE__, __LINE__, ##__VA_ARGS__)

int tun_alloc(const char *dev)
{
	struct ifreq ifr;
	int fd, err;


	if (dev == NULL) {
		error("Got NULL tun name");
		return -1;
	}


	fd = open("/dev/net/tun", O_RDWR);

	if (fd < 0) {
		error("Failed to open /dev/net/tun");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = IFF_TUN;
	if (*dev)
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
		error("Failed to create tun %s, error %d\n", dev, err);
		close(fd);
		return err;
	}
	return fd;
}

int main(int argc, char **argv)
{
	int rc = 0;
	int tun_fd = -1;
	system("modprobe -q tun");
	ssize_t nread;

	if ((tun_fd = tun_alloc(_TUN_NAME)) < 0) {
		error("Failed to create tun: %s\n", _TUN_NAME);
		exit(EXIT_FAILURE);
	}
	info("Tun '%s' created\n", _TUN_NAME);



	while(1) {
		nread = read(tun_fd, buffer, sizeof(buffer));
	}

out:
	if (fd >= 0)
		close(fd);

	exit(EXIT_SUCCESS);
}
