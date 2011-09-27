/*
 * Definitions for virtual stack
 */

////
// Constants
////

////
// Structures
////

typedef struct{
  int tap;
  char name[ETHERNET_NAME_MAX_SIZE];
  EthernetAddress address;
  } EthernetInterface;

////
// Prototypes
////

EthernetInterface *stackFindDeviceByAddr(EthernetAddress src);

