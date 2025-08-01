#include "MpsKeySearchDevice.h"
#include <iostream>

MpsKeySearchDevice::MpsKeySearchDevice(int deviceId, uint64_t keysPerStep) : _device(torch::kMPS) {
    if (!torch::mps::is_available()) {
        throw KeySearchException("MPS device not available");
    }
    this->_keysPerStep = keysPerStep;
    std::cout << "MpsKeySearchDevice created" << std::endl;
}

MpsKeySearchDevice::~MpsKeySearchDevice() {
    std::cout << "MpsKeySearchDevice destroyed" << std::endl;
}

void MpsKeySearchDevice::init(const secp256k1::uint256 &start, int compression, const secp256k1::uint256 &stride) {
    this->_startKey = start;
    this->_compression = compression;
    this->_stride = stride;
    std::cout << "MpsKeySearchDevice::init" << std::endl;
}

void MpsKeySearchDevice::doStep() {
    // This is where the magic happens
    // For now, just advance the key
    _startKey = _startKey + _stride;
}

void MpsKeySearchDevice::setTargets(const std::set<KeySearchTarget> &targets) {
    _targets.assign(targets.begin(), targets.end());
}

size_t MpsKeySearchDevice::getResults(std::vector<KeySearchResult> &results) {
    // Not implemented yet
    return 0;
}

uint64_t MpsKeySearchDevice::keysPerStep() {
    return _keysPerStep;
}

std::string MpsKeySearchDevice::getDeviceName() {
    return "MPS Key Search Device";
}

void MpsKeySearchDevice::getMemoryInfo(uint64_t &freeMem, uint64_t &totalMem) {
    // Not implemented for MPS
    freeMem = 0;
    totalMem = 0;
}

secp256k1::uint256 MpsKeySearchDevice::getNextKey() {
    return _startKey;
}
