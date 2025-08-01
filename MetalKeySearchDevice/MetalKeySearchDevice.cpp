#include "MetalKeySearchDevice.h"
#include "Logger.h"
#include "util.h"
#include <fstream>
#include <iostream>

MetalKeySearchDevice::MetalKeySearchDevice(int deviceId, uint64_t keysPerStep) {
    _device = MTL::CreateSystemDefaultDevice();
    if(!_device) {
        throw KeySearchException("Failed to create Metal device");
    }

    _commandQueue = _device->newCommandQueue();
    if(!_commandQueue) {
        throw KeySearchException("Failed to create Metal command queue");
    }

    std::string source = util::readTextFile("MetalKeySearchDevice/keysearch.metal");
    if(source.empty()) {
        throw KeySearchException("Failed to read metal kernel file");
    }

    NS::Error* error = nullptr;
    _library = _device->newLibrary(NS::String::string(source.c_str(), NS::UTF8StringEncoding), nullptr, &error);
    if(!_library) {
        throw KeySearchException("Failed to create Metal library: " + std::string(error->localizedDescription()->utf8String()));
    }

    _function = _library->newFunction(NS::String::string("generate_public_key", NS::UTF8StringEncoding));
    if(!_function) {
        throw KeySearchException("Failed to create Metal function");
    }

    _pipelineState = _device->newComputePipelineState(_function, &error);
    if(!_pipelineState) {
        throw KeySearchException("Failed to create Metal pipeline state: " + std::string(error->localizedDescription()->utf8String()));
    }

    this->_keysPerStep = keysPerStep;
    Logger::log(LogLevel::Info, "MetalKeySearchDevice created");
}

MetalKeySearchDevice::~MetalKeySearchDevice() {
    _pipelineState->release();
    _function->release();
    _library->release();
    _commandQueue->release();
    _device->release();
    Logger::log(LogLevel::Info, "MetalKeySearchDevice destroyed");
}

void MetalKeySearchDevice::init(const secp256k1::uint256 &start, int compression, const secp256k1::uint256 &stride) {
    this->_startKey = start;
    this->_compression = compression;
    this->_stride = stride;
    Logger::log(LogLevel::Info, "MetalKeySearchDevice initialized");
}

void MetalKeySearchDevice::doStep() {
    // Create buffers
    MTL::Buffer* privateKeysBuffer = _device->newBuffer(_keysPerStep * sizeof(uint256_t), MTL::ResourceStorageModeShared);
    MTL::Buffer* publicKeysXBuffer = _device->newBuffer(_keysPerStep * sizeof(uint256_t), MTL::ResourceStorageModeShared);
    MTL::Buffer* publicKeysYBuffer = _device->newBuffer(_keysPerStep * sizeof(uint256_t), MTL::ResourceStorageModeShared);

    // Generate private keys
    uint256_t* privateKeys = (uint256_t*)privateKeysBuffer->contents();
    for (uint64_t i = 0; i < _keysPerStep; i++) {
        privateKeys[i] = _startKey + _stride * i;
    }

    // Create a command buffer
    MTL::CommandBuffer* commandBuffer = _commandQueue->commandBuffer();
    MTL::ComputeCommandEncoder*- commandEncoder = commandBuffer->computeCommandEncoder();

    // Set pipeline state and buffers
    commandEncoder->setComputePipelineState(_pipelineState);
    commandEncoder->setBuffer(privateKeysBuffer, 0, 0);
    commandEncoder->setBuffer(publicKeysXBuffer, 0, 1);
    commandEncoder->setBuffer(publicKeysYBuffer, 0, 2);

    // Dispatch the kernel
    MTL::Size gridSize = MTL::Size(_keysPerStep, 1, 1);
    NS::UInteger threadGroupSize = _pipelineState->maxTotalThreadsPerThreadgroup();
    if (threadGroupSize > _keysPerStep) {
        threadGroupSize = _keysPerStep;
    }
    MTL::Size threadgroupSize = MTL::Size(threadGroupSize, 1, 1);
    commandEncoder->dispatchThreads(gridSize, threadgroupSize);

    // End encoding and commit the command buffer
    commandEncoder->endEncoding();
    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();

    // TODO: Read results and check for matches

    // Clean up
    privateKeysBuffer->release();
    publicKeysXBuffer->release();
    publicKeysYBuffer->release();

    _startKey = _startKey + _stride * _keysPerStep;
}

void MetalKeySearchDevice::setTargets(const std::set<KeySearchTarget> &targets) {
}

size_t MetalKeySearchDevice::getResults(std::vector<KeySearchResult> &results) {
    return 0;
}

uint64_t MetalKeySearchDevice::keysPerStep() {
    return 0;
}

std::string MetalKeySearchDevice::getDeviceName() {
    return "Metal Key Search Device";
}

void MetalKeySearchDevice::getMemoryInfo(uint64_t &freeMem, uint64_t &totalMem) {
    freeMem = 0;
    totalMem = 0;
}

secp256k1::uint256 MetalKeySearchDevice::getNextKey() {
    return secp256k1::uint256(0);
}
