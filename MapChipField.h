#pragma once
#include "KamataEngine.h"
#include <vector>

class MapChipField {
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
	MapChipData mapChipData_;
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	KamataEngine::Vector3 GetMapChipPositionBiIndex(uint32_t xIndex, uint32_t yIndex);
};
enum class MapChipType {
	kBlank,
	kBlock,
};
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};
