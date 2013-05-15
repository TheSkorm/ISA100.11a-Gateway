#!/bin/sh
#this file is left blank intentionally
#put here commands to be executed ONCE right after a fw update
#but before starting the new fw version (cleanups, etc...)

set -x

. common.sh
VerifyConsistentHwVer || exit 1

. build_info

if [ "$hw" = "vr900" ]; then
	DIR_BIN=/usr/bin/
	DIR_LIB=/lib/	
else
	#default
	DIR_BIN=/access_node/bin/
	DIR_LIB=/access_node/lib/
fi

local MODULES_RELOADED=0

# killall all ISA modules
killall -9 watchdog backbone sys_monitor.sh scheduler isa_gw SystemManager MonitorHost modbus_gw modbus_gw_isa sm_extra_logs_cleaner.sh sm_log_upload.sh scgi_svc

# killall extra WHart 
killall -9 whaccesspoint WHart_GW.o WHart_NM.o 

user_name=zero
echo "Add user $user_name"
tmp=`cat /etc/passwd | grep "^${user_name}:" | wc -l`
if [ $tmp -eq 0 ]; then
	echo "${user_name}:\$1\$\$aXYuEZMfDC0og07lll.CX1:0:0:${user_name}:/access_node/:/bin/sh" >> /etc/passwd

fi


# CGI/RPC specific section
user_name=nivis
echo "Add user $user_name"
tmp=`cat /etc/passwd | grep "^${user_name}:" | wc -l`
if [ $tmp -eq 0 ]; then
	echo "${user_name}:\$1\$\$CoERg7ynjYLsj2j4glJ34.:0:0:${user_name}:/access_node/firmware/:cfg.sh" >> /etc/passwd

fi

if [ ! -f /access_node/activity_files/userdb.ini ] ; then
	echo -e "[admin]\n\tpass = \$1\$\$GJd2atarym6kuMksRfc5S0" > "/access_node/activity_files/userdb.ini"
fi

if [ -f an_sys_patch/mini_httpd ]; then
	rm /access_node/bin/mini_httpd	#remove any possible previous instance
	mv an_sys_patch/mini_httpd ${DIR_BIN}mini_httpd
	chmod +x ${DIR_BIN}mini_httpd
fi

# is correct also for vr900?
[ -f an_sys_patch/access_node_cert.pem ] && mv an_sys_patch/access_node_cert.pem	/access_node/activity_files/access_node_cert.pem

# end of CGI/RPC section


killall mgetty
killall discovery
killall -9 discovery

[ -f an_sys_patch/pl2303.ko ]		&& rmmod pl2303
[ -f an_sys_patch/af_packet.ko ]	&& rmmod af_packet.ko
[ -f an_sys_patch/cp2101.ko ]	&& rmmod cp2101
[ -f an_sys_patch/ftdi_sio.ko ]		&&rmmod ftdi_sio

[ ! -d /access_node/lib/modules/2.6.20.7/net/packet ] && mkdir /access_node/lib/modules/2.6.20.7/net/packet
[ ! -d /etc/ntp ]    && mkdir /etc/ntp
[ ! -d /etc/udhcpc ] && mkdir /etc/udhcpc
[ ! -d /etc/config ] && mkdir /etc/config

mv an_sys_patch/peers/*			/access_node/etc/ppp/peers/

# cp only for kernel modules that must be reloaded at the end of this script
[ -f an_sys_patch/af_packet.ko ] 	&& cp an_sys_patch/af_packet.ko		/access_node/lib/modules/2.6.20.7/net/packet/
[ -f an_sys_patch/pl2303.ko ] 		&& cp an_sys_patch/pl2303.ko		/access_node/lib/modules/2.6.20.7/kernel/drivers/usb/serial/
[ -f an_sys_patch/ftdi_sio.ko ] 	&& cp an_sys_patch/ftdi_sio.ko		/access_node/lib/modules/2.6.20.7/kernel/drivers/usb/serial/
[ -f an_sys_patch/cp2101.ko ] 		&& cp an_sys_patch/cp2101.ko		/access_node/lib/modules/2.6.20.7/kernel/drivers/usb/serial/

[ -f an_sys_patch/ntp.conf ] 		&& mv an_sys_patch/ntp.conf			/etc/
[ -f an_sys_patch/rc.board ] 		&& mv an_sys_patch/rc.board			/etc/rc.d/
[ -f an_sys_patch/rc.local ] 		&& mv an_sys_patch/rc.local			/etc/rc.d/
[ -f an_sys_patch/rc.local.prod ] 	&& mv an_sys_patch/rc.local.prod	/etc/rc.d/
[ -f an_sys_patch/rc.nameservers ] 	&& mv an_sys_patch/rc.nameservers	/etc/rc.d/
[ -f an_sys_patch/rc.net ] 			&& mv an_sys_patch/rc.net			/etc/rc.d/
[ -f an_sys_patch/rc.rtc ] 			&& mv an_sys_patch/rc.rtc			/etc/rc.d/
[ -f an_sys_patch/udhcpc.sh ] 		&& mv an_sys_patch/udhcpc.sh		/etc/udhcpc/
[ -f an_sys_patch/ip-down ] 		&& mv an_sys_patch/ip-down			/etc/ppp/
[ -f an_sys_patch/ip-up ] 			&& mv an_sys_patch/ip-up			/etc/ppp/

[ -f an_sys_patch/hotplug ] 		&& mv an_sys_patch/hotplug			/access_node/sbin/

[ -f an_sys_patch/rc.iptables ] 	&& mv an_sys_patch/rc.iptables		/access_node/etc/rc.d/rc.iptables

[ -f an_sys_patch/login.config ] 	&& mv an_sys_patch/login.config		/access_node/mgetty/
[ -f an_sys_patch/mgetty ] 			&& mv an_sys_patch/mgetty			/access_node/mgetty/
[ -f an_sys_patch/ntpd ] 			&& mv an_sys_patch/ntpd				${DIR_BIN}/
[ -f an_sys_patch/ntpq ] 			&& mv an_sys_patch/ntpq				${DIR_BIN}/
[ -f an_sys_patch/i2c ] 			&& mv an_sys_patch/i2c				${DIR_BIN}/
[ -f an_sys_patch/tftp ] 			&& mv an_sys_patch/tftp				${DIR_BIN}/tftp



if [ -d an_sys_patch/ssl_resources/ ]; then 	
	
	if [ ! -d ${NIVIS_ACTIVITY_FILES}/ssl_resources/ ]; then 
		mv -f an_sys_patch/ssl_resources/		${NIVIS_ACTIVITY_FILES}		
	else
		#TODO: add logic to not override  if files exist 
		mv -f an_sys_patch/ssl_resources/*		${NIVIS_ACTIVITY_FILES}/ssl_resources/
	fi	
fi


if [ -f an_sys_patch/libaxtls.so.1.2 ]; then
	mv an_sys_patch/libaxtls.so.1.2	${DIR_LIB}
	rm -f ${DIR_LIB}/libaxtls.so.1
	ln -sf ${DIR_LIB}/libaxtls.so.1.2 ${DIR_LIB}/libaxtls.so.1
	#this is really ugly... consider adding /access_node/lib/ to LD_LIBRARY_PATH
	rm -f /access_node/firmware/lib/libaxtls.so.1
	ln -sf ${DIR_LIB}/libaxtls.so.1.2 /access_node/firmware/lib/libaxtls.so.1
fi

if [ -f an_sys_patch/libsqlite3.so ]; then
	mv an_sys_patch/libsqlite3.so ${DIR_LIB}libsqlite3.so
	chmod +x ${DIR_LIB}libsqlite3.so
	ln -sf ${DIR_LIB}libsqlite3.so ${DIR_LIB}libsqlite3.so.0
fi

ln -sf  /access_node/wwwroot/app /access_node/firmware/www/wwwroot/app

# /etc/resolv.conf -> /etc/ppp/resolv.conf -> /tmp/resolv.conf to avoid flash writes on ip up/down
ln -sf /tmp/resolv.conf /etc/ppp/resolv.conf
ln -sf /etc/ppp/resolv.conf /etc/resolv.conf

/access_node/firmware/ini_editor -s CC_COMM -v MODEM_PWR_LINE_INDEX -w 9

[ -x release-one_time.sh ] && release-one_time.sh

#do not care if  af_packet.ko  fails for now
insmod /access_node/lib/modules/2.6.20.7/net/packet/af_packet.ko

MODULES_RELOADED=1
if [ -f an_sys_patch/pl2303.ko ] ; then
	insmod /access_node/lib/modules/2.6.20.7/kernel/drivers/usb/serial/pl2303.ko || MODULES_RELOADED=0
	rm -f  an_sys_patch/pl2303.ko
fi

if [ -f an_sys_patch/cp2101.ko ]; then
	insmod /access_node/lib/modules/2.6.20.7/kernel/drivers/usb/serial/cp2101.ko || MODULES_RELOADED=0
	rm -f an_sys_patch/cp2101.ko
fi

if [ -f an_sys_patch/ftdi_sio.ko ]; then
	insmod /access_node/lib/modules/2.6.20.7/kernel/drivers/usb/serial/ftdi_sio.ko || MODULES_RELOADED=0
	rm -f an_sys_patch/ftdi_sio.ko
fi

update_profile.sh one_time 

[ -f ${NIVIS_PROFILE}/modbus_gw_isa.ini ] && mv ${NIVIS_PROFILE}/modbus_gw_isa.ini ${NIVIS_PROFILE}/modbus_gw.ini 
[ ! -d ${NIVIS_ACTIVITY_FILES}/devices_fw_upgrade/ ] && mkdir -p ${NIVIS_ACTIVITY_FILES}/devices_fw_upgrade/

#update ftp upload scripts firmware->access_node ONLY if the scripts are already used
#TODO: maybe use diff > /dev/null to avoid useless updating
[ -f /access_node/ftp_log_upload.sh ] && cp /access_node/firmware/ftp_log_upload.sh /access_node/ftp_log_upload.sh
[ -f /access_node/sm_log_upload.sh  ] && cp /access_node/firmware/sm_log_upload.sh  /access_node/sm_log_upload.sh

sleep 1
discovery&

if [ $MODULES_RELOADED -eq 0 ]; then
	log2flash "WARN: one_time.sh couldn't reload modules. Rebooting..."
	log2flash "one_time.sh executed"
	mv -f one_time.sh one_time.sh.EXECUTED
	sleep 1
	reboot
	sleep 2
	log2flash "ERR: one_time.sh: this should not be logged due to reboot."

fi

