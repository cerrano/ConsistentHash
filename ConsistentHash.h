/**
* @file ConsistentHash.h。
*
* 一致性Hash (無thread-safe)
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

// 節點類，包含IP和埠
class hashNode {
public:
	std::string ip;
	int port;

	// 構造函數，初始化IP和埠
	hashNode(const std::string &ip, int port) : ip(ip), port(port) {}

	// 返回節點的字串表示形式
	std::string toString() const {
		return ip + ":" + std::to_string(port);
	}

	// 重載等於運算子，用於節點比較
	bool operator==(const hashNode &other) const {
		return ip == other.ip && port == other.port;
	}
};

// 雜湊環類
class ConsistentHash {
private:
	// 雜湊環，存儲雜湊值到節點的映射
	std::map<int, hashNode> hashRing;
	// 每個實際節點對應的虛擬節點數量
	int virtualNodeCount;
	// 標準庫的雜湊函數物件
	std::hash<std::string> hashFunction;
public:
	// 構造函數，初始化虛擬節點數量
	ConsistentHash(int virtualNodeCount = 3) : virtualNodeCount(virtualNodeCount) {}

	// 將節點添加到雜湊環中
	void addNode(const hashNode &node) {
		for (int i = 0; i < virtualNodeCount; ++i) {
			// 為每個虛擬節點生成唯一的字串
			std::string virtualNode = node.toString() + "#" + std::to_string(i);
			// 計算虛擬節點的雜湊值
			int hash = hashFunction(virtualNode);
			// 將雜湊值和節點存儲到雜湊環中
			hashRing.emplace(std::make_pair(hash, node));
		}
	}

	// 根據session_id查找對應的節點
	hashNode getNode(unsigned int session_id) {
		// 如果雜湊環為空，拋出異常
		if (hashRing.empty()) {
			throw std::runtime_error("No nodes in the hash ring");
		}

		// 將session_id轉換為字串
		std::string key = std::to_string(session_id);
		// 計算鍵的雜湊值
		int hash = hashFunction(key);
		// 在雜湊環中找到第一個大於或等於該雜湊值的節點
		auto it = hashRing.lower_bound(hash);
		// 如果沒有找到，返回雜湊環中的第一個節點（形成環狀結構）
		if (it == hashRing.end()) {
			it = hashRing.begin();
		}
		return it->second; // 返回找到的節點
	}

	// 移除節點
	void removeNode(const hashNode &node) {
		for (int i = 0; i < virtualNodeCount; ++i) {
			// 為每個虛擬節點生成唯一的字串
			std::string virtualNode = node.toString() + "#" + std::to_string(i);
			// 計算虛擬節點的雜湊值
			int hash = hashFunction(virtualNode);
			// 從雜湊環中移除該虛擬節點
			hashRing.erase(hash);
		}
	}
};

#endif /*_CONSISTENT_HASH_H_*/