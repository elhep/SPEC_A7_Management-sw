QS93_IP=192.168.0.151
QS93_IP=10.60.0.216


install_config:
	scp -O -r wrnode_status_init root@$(QS93_IP):/root/.
	scp -O etc_config/* root@$(QS93_IP):/etc/config/.

install_etc_initd:
	scp -O etc_initd_scripts/* root@$(QS93_IP):/etc/init.d/.

install_hello:
	cd ./hello && make
	scp -O hello/hello root@$(QS93_IP):/root/.

install_devmem:
	cd ./devmem && make
	scp -O devmem/devmem root@$(QS93_IP):/root/.

install_spitest:
	cd ./spidev_test && make
	scp -O spidev_test/spidev_test root@$(QS93_IP):/root/.

install_scripts:
	scp -O -r scripts/* root@$(QS93_IP):/root/.

install_xvc:
	cd ./xvc && make
	scp -O xvc/build/xvc root@$(QS93_IP):/root/.

install_luci_app_wrnode:
	scp -O -r luci_www/luci-app-wrnode/root/* root@$(QS93_IP):/
	scp -O -r luci_www/luci-app-wrnode/htdocs/* root@$(QS93_IP):/www/

# before running install_all make sure that all wrnode management services are disabled
install_all: \
	install_config \
	install_etc_initd \
	install_scripts \
	install_xvc \
	install_luci_app_wrnode

clean_all:
	cd ./xvc_server_daemon && make clean
