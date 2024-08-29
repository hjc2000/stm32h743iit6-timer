#pragma once
#include <base/HandleWrapper.h>
#include <hal.h>
#include <stdint.h>

namespace hal
{
    /// @brief 看门狗初始化选项。
    class IndependentWatchDogConfig :
        public base::HandleWrapper<IWDG_InitTypeDef>
    {
    private:
        IWDG_InitTypeDef _config_handle{};

    public:
        IndependentWatchDogConfig();
        IndependentWatchDogConfig(IWDG_InitTypeDef const &o);
        IndependentWatchDogConfig &operator=(IWDG_InitTypeDef const &o);

        IWDG_InitTypeDef &Handle() override;

#pragma region 分频系数
        /// @brief 看门狗预分频器分频系数
        enum class PrescalerOption
        {
            Div4 = IWDG_PRESCALER_4,
            Div8 = IWDG_PRESCALER_8,
            Div16 = IWDG_PRESCALER_16,
            Div32 = IWDG_PRESCALER_32,
            Div64 = IWDG_PRESCALER_64,
            Div128 = IWDG_PRESCALER_128,
            Div256 = IWDG_PRESCALER_256,
        };

        /// @brief 看门狗预分频器分频系数。
        /// @return
        PrescalerOption Prescaler() const;
        void SetPrescaler(PrescalerOption value);

        /// @brief 根据 2 的整数幂获取分频系数。例如 PrescalerOption::Div8
        /// 是 2 的 3 次幂，于是返回 3.
        /// @return
        uint8_t GetPrescalerByPow() const;
        void SetPrescalerByPow(uint8_t pow);

        /// @brief 获取 uint32_t 类型的分频系数，分多少频，整数值就是多少。
        /// @return
        uint32_t GetPrescalerByUint32() const;
        void SetPrescalerByUint32(uint32_t value);
#pragma endregion

        /// @brief 看门狗重载值。允许的范围：[0, 0x0FFF]
        /// @return
        uint32_t ReloadValue() const;
        void SetReloadValue(uint32_t value);

        uint32_t WindowValue() const;
        void SetWindowValue(uint32_t value);
    };
} // namespace hal
