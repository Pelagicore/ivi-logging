#pragma once

/**
 * This class provides the same interface as other LogData classes, but actually does nothing with the objects which are streamed to it.
 * It can be useful to disable some logging.
 */
class NullLogData {
public:
	template<typename DataType>
	NullLogData& operator<<(const DataType& v) {
		return *this;
	}

};
