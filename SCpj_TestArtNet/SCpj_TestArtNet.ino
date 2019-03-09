/*
 Name:		SCpj_TestArtNet.ino
 Created:	08-Mar-19 20:04:27
 Author:	statu
*/

#include <SClb_Arduino_DMXEthernet.h>
#include <SClb_ArtNetNode1.h>
#include <SClb_ArtNetNode2.h>
#include <SClb_ArtNetNode3.h>
#include <SClb_ArtNetNode4.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define USE_DHCP 1

#define MAC_ADDRESS 0x90, 0xA2, 0xDA, 0x10, 0x6C, 0xA8
#define IP_ADDRESS 192,168,0,77
#define GATEWAY_IP 192,168,0,1
#define SUBNET_MASK 255,255,255,0
#define BROADCAST_IP 192,168,0,255

#define ARTNET_NODE_NAME "STATUCUE ArtNET"

#define SDSELECT_PIN 4

//*********************** globals ***********************

//network addresses
byte mac[] = { MAC_ADDRESS };
IPAddress ip(IP_ADDRESS);
IPAddress gateway(GATEWAY_IP);
IPAddress subnet_mask(SUBNET_MASK);

#if defined( BROADCAST_IP )
IPAddress broadcast_ip(BROADCAST_IP);
#else
IPAddress broadcast_ip = INADDR_NONE;
#endif

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP eUDP;

uint8_t artnetBuffer[ARTNET_BUFFER_MAX];
SCArtNet1* artNetInterface1;
SCArtNet2* artNetInterface2;
SCArtNet3* artNetInterface3;
SCArtNet4* artNetInterface4;


// the setup function runs once when you press reset or power the board
void setup() {
#if defined(SDSELECT_PIN)
	pinMode(SDSELECT_PIN, OUTPUT);
#endif
	// Initialize Ethernet
	if (USE_DHCP) {
		Ethernet.begin(mac);										// DHCP
	}
	else {
		Ethernet.begin(mac, ip, gateway, gateway, subnet_mask);		// Static
	}

	artNetInterface1 = new SCArtNet1(Ethernet.localIP(), Ethernet.subnetMask(), artnetBuffer);
	artNetInterface1->setSubnetUniverse(0, 0);

	artNetInterface2 = new SCArtNet2(Ethernet.localIP(), Ethernet.subnetMask(), artnetBuffer);
	artNetInterface2->setSubnetUniverse(0, 0);

	artNetInterface3 = new SCArtNet3(Ethernet.localIP(), Ethernet.subnetMask(), artnetBuffer);
	artNetInterface3->setSubnetUniverse(0, 0);

	artNetInterface4 = new SCArtNet4(Ethernet.localIP(), Ethernet.subnetMask(), artnetBuffer);
	artNetInterface4->setSubnetUniverse(0, 0);

	eUDP.begin(artNetInterface1->dmxPort());
	((SCArtNet1*)artNetInterface1)->setNodeName(ARTNET_NODE_NAME);
	((SCArtNet1*)artNetInterface1)->send_art_poll_reply(&eUDP);

	eUDP.begin(artNetInterface2->dmxPort());
	((SCArtNet2*)artNetInterface2)->setNodeName(ARTNET_NODE_NAME);
	((SCArtNet2*)artNetInterface2)->send_art_poll_reply(&eUDP);

	eUDP.begin(artNetInterface3->dmxPort());
	((SCArtNet3*)artNetInterface3)->setNodeName(ARTNET_NODE_NAME);
	((SCArtNet3*)artNetInterface3)->send_art_poll_reply(&eUDP);

	eUDP.begin(artNetInterface4->dmxPort());
	((SCArtNet4*)artNetInterface4)->setNodeName(ARTNET_NODE_NAME);
	((SCArtNet4*)artNetInterface4)->send_art_poll_reply(&eUDP);

}

// the loop function runs over and over again until power down or reset
void loop() {
	// Send DMX
	uint16_t packetSize = eUDP.parsePacket();

	if (packetSize) {
		packetSize = eUDP.read(artnetBuffer, ARTNET_BUFFER_MAX);
		uint8_t read_result_artnet1 = artNetInterface1->readDMXPacketContents(&eUDP, packetSize);
		uint8_t read_result_artnet2 = artNetInterface2->readDMXPacketContents(&eUDP, packetSize);
		uint8_t read_result_artnet3 = artNetInterface3->readDMXPacketContents(&eUDP, packetSize);
		uint8_t read_result_artnet4 = artNetInterface4->readDMXPacketContents(&eUDP, packetSize);

	}
}
