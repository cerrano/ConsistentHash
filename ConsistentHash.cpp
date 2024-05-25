#include "ConsistentHash.h"

void testConsistentHash() {
	// �Ыؤ@�P�����ꪫ��
	ConsistentHash consistentHash;

	// �K�[�`�I
	consistentHash.addNode(hashNode("192.168.0.1", 8989));
	consistentHash.addNode(hashNode("192.168.0.2", 8989));
	consistentHash.addNode(hashNode("192.168.0.3", 8989));
	consistentHash.addNode(hashNode("192.168.0.4", 8989));

	// ��� session_id = 1 �M�g���`�I
	hashNode node1 = consistentHash.getNode(1);
	std::cout << "Session ID: 1 is initially mapped to node: " << node1.toString() << std::endl;

	// �����`�I 192.168.0.3:8989
	consistentHash.removeNode(hashNode("192.168.0.3", 8989));

	// ��� session_id = 1 �M�g���`�I
	hashNode node2 = consistentHash.getNode(1);
	std::cout << "After removal, session ID: 1 is mapped to node: " << node2.toString() << std::endl;

	// ���s�K�[�`�I 192.168.0.3:8989
	consistentHash.addNode(hashNode("192.168.0.3", 8989));

	// ��� session_id = 1 �M�g���`�I
	hashNode node3 = consistentHash.getNode(1);
	std::cout << "After re-adding, session ID: 1 is mapped to node: " << node3.toString() << std::endl;

}