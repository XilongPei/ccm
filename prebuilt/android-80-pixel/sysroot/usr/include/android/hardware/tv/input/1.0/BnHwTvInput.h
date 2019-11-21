#ifndef HIDL_GENERATED_ANDROID_HARDWARE_TV_INPUT_V1_0_BNHWTVINPUT_H
#define HIDL_GENERATED_ANDROID_HARDWARE_TV_INPUT_V1_0_BNHWTVINPUT_H

#include <android/hardware/tv/input/1.0/IHwTvInput.h>

namespace android {
namespace hardware {
namespace tv {
namespace input {
namespace V1_0 {

struct BnHwTvInput : public ::android::hidl::base::V1_0::BnHwBase {
    explicit BnHwTvInput(const ::android::sp<ITvInput> &_hidl_impl);
    explicit BnHwTvInput(const ::android::sp<ITvInput> &_hidl_impl, const std::string& HidlInstrumentor_package, const std::string& HidlInstrumentor_interface);

    ::android::status_t onTransact(
            uint32_t _hidl_code,
            const ::android::hardware::Parcel &_hidl_data,
            ::android::hardware::Parcel *_hidl_reply,
            uint32_t _hidl_flags = 0,
            TransactCallback _hidl_cb = nullptr) override;

    ::android::sp<ITvInput> getImpl() { return _hidl_mImpl; };
private:
    // Methods from ITvInput follow.

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    ::android::hardware::Return<void> ping();
    using getDebugInfo_cb = ::android::hidl::base::V1_0::IBase::getDebugInfo_cb;
    ::android::hardware::Return<void> getDebugInfo(getDebugInfo_cb _hidl_cb);

    ::android::sp<ITvInput> _hidl_mImpl;
};

}  // namespace V1_0
}  // namespace input
}  // namespace tv
}  // namespace hardware
}  // namespace android

#endif  // HIDL_GENERATED_ANDROID_HARDWARE_TV_INPUT_V1_0_BNHWTVINPUT_H