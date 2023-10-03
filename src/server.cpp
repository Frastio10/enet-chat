#include <cstring>
#include <enet/enet.h>
#include <iostream>

int main() {
  if (enet_initialize() != 0) {
    fprintf(stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }

  atexit(enet_deinitialize);

  ENetHost *client;
  ENetAddress address;

  enet_address_set_host(&address, "localhost");
  address.port = 1234;

  client = enet_host_create(nullptr, 1, 2, 0, 0);

  if (client == nullptr) {
    fprintf(stderr,
            "An error occurred while trying to create an ENet client host.\n");
    return EXIT_FAILURE;
  }

  ENetPeer *peer = enet_host_connect(client, &address, 2, 0);

  if (peer == nullptr) {
    fprintf(stderr, "No available peers for initiating an ENet connection.\n");
    return EXIT_FAILURE;
  }

  ENetEvent event;

  if (enet_host_service(client, &event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT) {
    std::cout << "Connected to the server." << std::endl;

    // Send a message to the server
    ENetPacket *packet =
        enet_packet_create("Hello, server!", strlen("Hello, server!") + 1,
                           ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
  } else {
    std::cout << "Connection to the server failed." << std::endl;
  }

  while (true) {
    while (enet_host_service(client, &event, 1000) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "Received a message from the server: "
                  << event.packet->data << std::endl;
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "Disconnected from the server." << std::endl;
        return EXIT_SUCCESS;

      default:
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
