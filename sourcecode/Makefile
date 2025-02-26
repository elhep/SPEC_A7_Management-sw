QS93_IP=192.168.0.17


install_config:
	# scp -r wrnode_status_init root@$(QS93_IP):/root/.
	scp etc_config/* root@$(QS93_IP):/etc/config/.

install_etc_initd:
	scp etc_initd_scripts/* root@$(QS93_IP):/etc/init.d/.

install_hello:
	cd ./hello && make
	scp hello/hello root@$(QS93_IP):/root/.

install_scripts:
	scp -r scripts/* root@$(QS93_IP):/root/.

install_xvc:
	cd ./xvc_server_daemon && make
	scp xvc_server_daemon/xvc root@$(QS93_IP):/root/.

install_luci_app_wrnode:
	scp -r luci_www/luci-app-wrnode/root/* root@$(QS93_IP):/
	scp -r luci_www/luci-app-wrnode/htdocs/* root@$(QS93_IP):/www/

# before running install_all make sure that all wrnode management services are disabled
install_all: \
	install_config \
	install_etc_initd \
	install_scripts \
	install_xvc \
	install_luci_app_wrnode

clean_all:
	cd ./xvc_server_daemon && make clean
