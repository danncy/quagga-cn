#include "board.h"
#include "port.h"

namespace dxnet {

Board::Board(const std::string& name, const std::string& addr)
  : name_(name),
    addr_(addr) {
     
}

Board::~Board() {
  ports_.clear();
}

void Board::appendPort(std::unique_ptr<Port> port) {
  ports_.push_back(std::move(port));
}

void Board::removePort(uint8_t index) {
  if (index >= portNum())
    return;
  auto it = ports_.erase(ports_.begin() + index);
  it->reset();
}

Port* Board::getPort(uint8_t index) {
  if (index >= portNum())
    return nullptr;

  return ports_[index].get();
}

int Board::portNum() {
  return ports_.size();
}

std::ostream& operator << (std::ostream& os, const Board& board) {
  return os << board.name_
            << "   HWaddr "
            << board.addr_
            << std::endl;
}

bool operator == (const Board& bda, const Board& bdb) {
  return bda.name_ == bdb.name_ &&
         bda.addr_ == bdb.addr_;
}

}
