#pragma once

#include <array>
#include <future>
#include "behaviortree_cpp/loggers/abstract_logger.h"

namespace BT
{
class Groot2Publisher : public StatusChangeLogger
{
  static std::mutex used_ports_mutex;
  static std::set<unsigned> used_ports;

  public:
  Groot2Publisher(const BT::Tree& tree, unsigned server_port = 1667);

  ~Groot2Publisher() override;

  private:
  void callback(Duration timestamp,
                const TreeNode& node,
                NodeStatus prev_status,
                NodeStatus status) override;

  void flush() override;

  void threadLoop();

  void updateStatusBuffer();

  std::vector<uint8_t> generateBlackBoardsDump(const std::string& bb_list);

  unsigned server_port_ = 0;
  std::string server_address_;

  std::string tree_xml_;

  std::atomic_bool active_server_;
  std::thread thread_;

  std::mutex status_mutex_;

  std::unordered_map<uint16_t, char*> buffer_ptr_;
  std::string status_buffer_;

  // weak reference to the tree.
  std::unordered_map<std::string, std::weak_ptr<BT::Tree::Subtree>> subtrees_;

  struct Pimpl;
  Pimpl* zmq_;
};
}   // namespace BT

