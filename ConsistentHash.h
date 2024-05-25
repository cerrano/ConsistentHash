/**
* @file ConsistentHash.h�C
*
* �@�P��Hash (�Lthread-safe)
*
* @author boris.chen@localhost
*/

#ifndef _CONSISTENT_HASH_H_
#define _CONSISTENT_HASH_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>

// �`�I���A�]�tIP�M��
class hashNode {
public:
	std::string ip;
	int port;

	// �c�y��ơA��l��IP�M��
	hashNode(const std::string &ip, int port) : ip(ip), port(port) {}

	// ��^�`�I���r���ܧΦ�
	std::string toString() const {
		return ip + ":" + std::to_string(port);
	}

	// ��������B��l�A�Ω�`�I���
	bool operator==(const hashNode &other) const {
		return ip == other.ip && port == other.port;
	}
};

// ��������
class ConsistentHash {
private:
	// �������A�s�x����Ȩ�`�I���M�g
	std::map<int, hashNode> hashRing;
	// �C�ӹ�ڸ`�I�����������`�I�ƶq
	int virtualNodeCount;
	// �зǮw�������ƪ���
	std::hash<std::string> hashFunction;
public:
	// �c�y��ơA��l�Ƶ����`�I�ƶq
	ConsistentHash(int virtualNodeCount = 3) : virtualNodeCount(virtualNodeCount) {}

	// �N�`�I�K�[����������
	void addNode(const hashNode &node) {
		for (int i = 0; i < virtualNodeCount; ++i) {
			// ���C�ӵ����`�I�ͦ��ߤ@���r��
			std::string virtualNode = node.toString() + "#" + std::to_string(i);
			// �p������`�I�������
			int hash = hashFunction(virtualNode);
			// �N����ȩM�`�I�s�x����������
			hashRing.emplace(std::make_pair(hash, node));
		}
	}

	// �ھ�session_id�d��������`�I
	hashNode getNode(unsigned int session_id) {
		// �p�G���������šA�ߥX���`
		if (hashRing.empty()) {
			throw std::runtime_error("No nodes in the hash ring");
		}

		// �Nsession_id�ഫ���r��
		std::string key = std::to_string(session_id);
		// �p���䪺�����
		int hash = hashFunction(key);
		// �b�����������Ĥ@�Ӥj��ε��������Ȫ��`�I
		auto it = hashRing.lower_bound(hash);
		// �p�G�S�����A��^�����������Ĥ@�Ӹ`�I�]�Φ��������c�^
		if (it == hashRing.end()) {
			it = hashRing.begin();
		}
		return it->second; // ��^��쪺�`�I
	}

	// �����`�I
	void removeNode(const hashNode &node) {
		for (int i = 0; i < virtualNodeCount; ++i) {
			// ���C�ӵ����`�I�ͦ��ߤ@���r��
			std::string virtualNode = node.toString() + "#" + std::to_string(i);
			// �p������`�I�������
			int hash = hashFunction(virtualNode);
			// �q�������������ӵ����`�I
			hashRing.erase(hash);
		}
	}
};

#endif /*_CONSISTENT_HASH_H_*/