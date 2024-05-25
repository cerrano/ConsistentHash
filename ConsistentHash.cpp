#include "ConsistentHash.h"

void testConsistentHash() {
	// 創建一致性雜湊物件
	ConsistentHash consistentHash;

	// 添加節點
	consistentHash.addNode(hashNode("192.168.0.1", 8989));
	consistentHash.addNode(hashNode("192.168.0.2", 8989));
	consistentHash.addNode(hashNode("192.168.0.3", 8989));
	consistentHash.addNode(hashNode("192.168.0.4", 8989));

	// 獲取 session_id = 1 映射的節點
	hashNode node1 = consistentHash.getNode(1);
	std::cout << "Session ID: 1 is initially mapped to node: " << node1.toString() << std::endl;

	// 移除節點 192.168.0.3:8989
	consistentHash.removeNode(hashNode("192.168.0.3", 8989));

	// 獲取 session_id = 1 映射的節點
	hashNode node2 = consistentHash.getNode(1);
	std::cout << "After removal, session ID: 1 is mapped to node: " << node2.toString() << std::endl;

	// 重新添加節點 192.168.0.3:8989
	consistentHash.addNode(hashNode("192.168.0.3", 8989));

	// 獲取 session_id = 1 映射的節點
	hashNode node3 = consistentHash.getNode(1);
	std::cout << "After re-adding, session ID: 1 is mapped to node: " << node3.toString() << std::endl;

}