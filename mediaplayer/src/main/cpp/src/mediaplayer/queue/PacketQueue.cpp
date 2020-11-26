//
// Created by 听见 on 2020/9/29.
//

#include "PacketQueue.h"
Packet::Packet(AVPacket &packet, uint32_t serial) : packet(AVPacket()), serial(serial) {
  av_packet_ref(&this->packet, &packet);
}
Packet::~Packet() {
  av_packet_unref(&this->packet);
}
PacketQueue::PacketQueue(int64_t maxSize, uint32_t maxPacket)
    : maxPackNb(maxSize), maxPacket(maxPacket), emptyCond(mutex), fullCond(mutex) {

}
PacketQueue::~PacketQueue() {
  clear();
}

int8_t PacketQueue::put(AVPacket &packet, uint32_t serial) {
  mutex.lock();
  packets.emplace_back(packet, serial);
  nb_packet++;
  size += packet.size;
  duration += packet.duration;
  emptyCond.signal();
  mutex.unLock();
  return 0;
}
int8_t PacketQueue::take(AVPacket &packet, uint32_t *serial) {
  int8_t ret = 1;
  mutex.lock();
  while (!abort_request && packets.empty()) {
    emptyCond.wait();
  }
  if (abort_request) {
    ret = -1;
  }
  if (packets.empty()) {
    ret = 0;
  }
  av_packet_move_ref(&packet, &packets.front().packet);
  size -= packets.front().packet.size;
  duration -= packets.front().packet.duration;
  packets.pop_front();
  nb_packet--;
  mutex.unLock();
  return ret;
}
void PacketQueue::clear() {
  mutex.lock();
  packets.clear();
  auto begin = packets.begin();
  while (begin != packets.end()) {
    av_packet_unref(&begin->packet);
    begin++;
  }
  size = 0;
  nb_packet = 0;
  duration = 0;
  emptyCond.signal();
  mutex.unLock();
}
void PacketQueue::abort() {
  mutex.lock();
  abort_request = true;
  emptyCond.signal();
  mutex.unLock();
}

