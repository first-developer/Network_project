/*
 * Code for virtual machine
 */

////
// Include files
////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/if_tun.h>

#include <libarrays.h>
#include <libevents.h>
#include <libtap.h>

#include "netether.h"
#include "stack.h"

////
// Constants
////
#define BUFFER_SIZE                     1000
#define	ETHERNET_PROTO_ARP		0x0806
#define ETHERNET_STRING_MAX		18

////
// Global variables
////

static EthernetInterface interfaces[]={
  {-1,"eth0",{{0x00,0x01,0x02,0x03,0x04,0x05}}},
  {-1,"",}
  };


////
// Functions
////

EthernetInterface *stackFindDeviceByAddr(EthernetAddress src){
int i=0;
while(interfaces[i].name[0]!='\0'){
  if(ethernetCompare(interfaces[i].address,src))
    return interfaces+i;
  i++;
  }
return NULL;
}


// is_well_done: handle error when something is well or bad create/set 
void is_well_done ( int return_value, char* message ) {
  if ( (return_value == -1) ) { 
    perror(message); 
    exit(1);
  }
}


////
// Main procedure
////


int main(void){

  // Variables 
  long timeout; 
  
  // 3.5 (part 1)
  // ==============================================================================
	
  //set the interface 'tap0'			
  char interface[BUFFER_SIZE];
  strcpy(interface, "tap0");

  // get he file descriptor from this interface
  int fd_tap = allocateNetworkDevice(interface,IFF_TAP|IFF_NO_PI);
  // Verify if the allocation has been done perfectly
  is_well_done(fd_tap," 'AllocateNetworkDevice' failed for the 'TAP' interface\n ");

  //set tap0 descriptor
  interfaces[0].tap=fd_tap;

  // Create a receive packet event
  int evt = eventsCreate(1, &(interfaces[0]));
  // Verify if the receive packet event has been successful created 
  is_well_done(evt, " receive packet event creation failed ");
  
  // Associate a selector to the event created 'evt'
  int fd_evt_assoc = eventsAssociateDescriptor( evt, fd_tap, NULL);
	
  // Assigned the decode actions to 'evt'
  int evt_action = eventsAddAction( evt, ethernetDecodePacket, 0);
  // Verify if the action assigment is well done 
  is_well_done(evt_action, " Action 'EthernetDecodePacket' failed  in adding ");
  

  // 3.5 (part 2)
	// ==============================================================================
  
  // init data assoc array and setting data field 
  AssocArray *packet = createAssocArray();
  arraysSetValue(&packet, "data", (void *)"abcdefghijklmnopq", 18, AARRAY_DONT_DUPLICATE);
  
  // Setting dst field
  EthernetAddress dst = {{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}};
  arraysSetValue(&packet, "dst", &dst, sizeof(EthernetAddress), 0);
  
  // Setting src field
  EthernetAddress src= interfaces[0].address;
  arraysSetValue(&packet, "src", &src, sizeof(EthernetAddress), 0);
  
  // Setting protocole (ex: ARP)
  int proto = ETHERNET_PROTO_ARP;
  arraysSetValue(&packet, "proto", &proto, sizeof(int), 0);
  
  // Setting size of ethernet packet (default: ETHERNET_STRING_MAX = 18)
  int size = ETHERNET_STRING_MAX;
  arraysSetValue(&packet, "size", &size, sizeof(int), 0);

  // Create a send packet event
  int evt_send = eventsCreate(2, packet);
  // Verify if the send packet event has been successful created 
  is_well_done(evt_send, " send packet event creation failed ");
  
  // Assigned the send packet action to the evt_send   
  int evt_send_action = eventsAddAction(evt_send, ethernetSendPacket, 1);
  // Verify if the action assigment is well done 
  is_well_done(evt_send_action, " Action 'ethernetSendPacket' failed  in adding ");
  
  // Add event selector with a timeout of 30s to the evt_send
  timeout = 30000;
  int evt_send_schedule_selector = eventsSchedule(evt_send, timeout, packet);
  is_well_done(evt_send_action, " Setting of Schedule selector for the send event failed ");
  

  // Scanevents
  eventsScan();
	
  exit(0);
  
}
