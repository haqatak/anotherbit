#ifndef _METAL_KEY_SEARCH_DEVICE_H
#define _METAL_KEY_SEARCH_DEVICE_H

#include "KeySearchDevice.h"
#include <Metal/Metal.hpp>

class MetalKeySearchDevice : public KeySearchDevice {
private:
    MTL::Device* _device;
    MTL::CommandQueue* _commandQueue;
    MTL::Library* _library;
    MTL::Function* _function;
    MTL::ComputePipelineState* _pipelineState;

    secp256k1::uint256 _startKey;
    secp256k1::uint256 _stride;
    int _compression;
    std::vector<KeySearchTarget> _targets;
    uint64_t _keysPerStep;

public:
    MetalKeySearchDevice(int deviceId, uint64_t keysPerStep);
    virtual ~MetalKeySearchDevice();

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
