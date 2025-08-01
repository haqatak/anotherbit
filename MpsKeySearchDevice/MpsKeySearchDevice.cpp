#include "MpsKeySearchDevice.h"
#include "Logger.h"

MpsKeySearchDevice::MpsKeySearchDevice(int deviceId, uint64_t keysPerStep) : _device(torch::kMPS) {
    if (!torch::mps::is_available()) {
        throw KeySearchException("MPS device not available");
    }
    this->_keysPerStep = keysPerStep;
    Logger::log(LogLevel::Info, "MpsKeySearchDevice created");
}

MpsKeySearchDevice::~MpsKeySearchDevice() {
    Logger::log(LogLevel::Info, "MpsKeySearchDevice destroyed");
}

void MpsKeySearchDevice::init(const secp256k1::uint256 &start, int compression, const secp256k1::uint256 &stride) {
    this->_startKey = start;
    this->_compression = compression;
    this->_stride = stride;
    Logger::log(LogLevel::Info, "MpsKeySearchDevice initialized");
}

void MpsKeySearchDevice::doStep() {
    // TODO: Implement the core MPS computation by performing
    // elliptic curve point multiplication using MPS or PyTorch tensors, generate
    // addresses from the resulting public keys, apply the target matching logic to
    // check for desired keys, and collect any matching results. Replace the
    // placeholder increment with this full cryptographic processing workflow.
    _startKey = _startKey + _stride;
}

void MpsKeySearchDevice::setTargets(const std::set<KeySearchTarget> &targets) {
    _targets.assign(targets.begin(), targets.end());
}

size_t MpsKeySearchDevice::getResults(std::vector<KeySearchResult> &results) {
    // TODO: Implement this method to populate the results
    // vector with the found KeySearchResult objects and return the count of these
    // results to correctly report matches found by the device.
    return 0;
}

uint64_t MpsKeySearchDevice::keysPerStep() {
    return _keysPerStep;
}

std::string MpsKeySearchDevice::getDeviceName() {
    return "MPS Key Search Device";
}

void MpsKeySearchDevice::getMemoryInfo(uint64_t &freeMem, uint64_t &totalMem) {
    // TODO: Query actual MPS memory if PyTorch provides API
    // For now, return reasonable defaults or query system memory
    freeMem = 0;
    totalMem = 0;
}

secp256k1::uint256 MpsKeySearchDevice::getNextKey() {
    return _startKey;
}
