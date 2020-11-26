//
// Created by 听见 on 2020/9/29.
//

#ifndef MUSICPLAYER_PACKETQUEUE_H
#define MUSICPLAYER_PACKETQUEUE_H

#include "Mutex.h"
#include "Condition.h"
#include <list>
extern "C" {
#include "libavcodec/avcodec.h"
}
class Packet {

 public:
  AVPacket packet;
  uint32_t serial = 0;
  Packet(AVPacket &packet, uint32_t serial);
  virtual ~Packet();
};

class PacketQueue {
 private:
  uint32_t maxPacket;
  Mutex mutex{};
  Condition emptyCond;
  Condition fullCond;
  std::list<Packet> packets{};

  bool abort_request = false;
  uint32_t serial = 0;

 public:
  uint32_t size = 0;
  uint32_t nb_packet = 0;
  int64_t duration = 0;
  int64_t maxPackNb;

  PacketQueue(int64_t maxSize = 50 * 1000 * 100, uint32_t maxPacket = 25);
  ~PacketQueue();
  int8_t put(AVPacket &packet, uint32_t serial);
  int8_t take(AVPacket &packet, uint32_t *serial);
  void abort();
  void clear();

};

#endif //MUSICPLAYER_PACKETQUEUE_H
