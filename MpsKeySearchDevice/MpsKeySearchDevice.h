#ifndef _MPS_KEY_SEARCH_DEVICE_H
#define _MPS_KEY_SEARCH_DEVICE_H

#include "KeySearchDevice.h"
#include <torch/torch.h>

class MpsKeySearchDevice : public KeySearchDevice {
private:
    torch::Device _device;
    secp256k1::uint256 _startKey;
    secp256k1::uint256 _stride;
    int _compression;
    std::vector<KeySearchTarget> _targets;
    uint64_t _keysPerStep;

public:
    MpsKeySearchDevice(int deviceId, uint64_t keysPerStep);
    virtual ~MpsKeySearchDevice();

    virtual void init(const secp256k1::uint256 &start, int compression, const secp256k1::uint256 &stride);
    virtual void doStep();
    virtual void setTargets(const std::set<KeySearchTarget> &targets);
    virtual size_t getResults(std::vector<KeySearchResult> &results);
    virtual uint64_t keysPerStep();
    virtual std::string getDeviceName();
    virtual void getMemoryInfo(uint64_t &freeMem, uint64_t &totalMem);
    virtual secp256k1::uint256 getNextKey();
};

#endif
