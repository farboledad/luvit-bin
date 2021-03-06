/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 11805 2005-01-07 09:37:18Z dts12 $
 */
#ifndef CLOUDGATEPRIVATE_H
#define CLOUDGATEPRIVATE_H

/* function declarations */
void init_cloudGatePrivate(void);
Netsnmp_Node_Handler handle_upTime;
Netsnmp_Node_Handler handle_hostName;
Netsnmp_Node_Handler handle_currentTime;
Netsnmp_Node_Handler handle_serialNumber;
Netsnmp_Node_Handler handle_sysLoad1;
Netsnmp_Node_Handler handle_sysLoad5;
Netsnmp_Node_Handler handle_sysLoad15;
Netsnmp_Node_Handler handle_firmVer;
Netsnmp_Node_Handler handle_AssociatedClients;
Netsnmp_Node_Handler handle_ClientAssociations;
Netsnmp_Node_Handler handle_ClientDisassociations;
Netsnmp_Node_Handler handle_essid;
Netsnmp_Node_Handler handle_wlanMAC;
Netsnmp_Node_Handler handle_wlanFreq;
Netsnmp_Node_Handler handle_wlanChan;
Netsnmp_Node_Handler handle_apn;
Netsnmp_Node_Handler handle_internetStatus;
Netsnmp_Node_Handler handle_ipAddress;
Netsnmp_Node_Handler handle_bCastAddress;
Netsnmp_Node_Handler handle_netMask;
Netsnmp_Node_Handler handle_csRegistration;
Netsnmp_Node_Handler handle_psRegistration;
Netsnmp_Node_Handler handle_imei;
Netsnmp_Node_Handler handle_imsi;
Netsnmp_Node_Handler handle_firmMode;
Netsnmp_Node_Handler handle_carrier;
Netsnmp_Node_Handler handle_rssi;
Netsnmp_Node_Handler handle_ecio;
Netsnmp_Node_Handler handle_bandClass;
Netsnmp_Node_Handler handle_serviceType;
Netsnmp_Node_Handler handle_rfChannel;
Netsnmp_Node_Handler handle_plmn;
Netsnmp_Node_Handler handle_csCellIDandLac;
Netsnmp_Node_Handler handle_psCellIDandLac;
Netsnmp_Node_Handler handle_pinStatus;
Netsnmp_Node_Handler handle_pinRetries;
Netsnmp_Node_Handler handle_pukRetries;
Netsnmp_Node_Handler handle_rxPackets;
Netsnmp_Node_Handler handle_rxBytes;
Netsnmp_Node_Handler handle_txPackets;
Netsnmp_Node_Handler handle_txBytes;

#endif /* CLOUDGATEPRIVATE_H */
