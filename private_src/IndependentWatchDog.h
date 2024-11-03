#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <base/unit/Hz.h>
#include <base/unit/Seconds.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/timer/IIndependentWatchDog.h>
#include <chrono>
#include <IndependentWatchDogConfig.h>

namespace hal
{
    /// @brief 独立看门狗。
    /// @note 所谓独立看门狗就是具有自己的内部时钟源，不依赖单片机的系统时钟。
    /// 在系统时钟失效时仍然能工作。
    class IndependentWatchDog :
        public bsp::IIndependentWatchDog
    {
    private:
        IWDG_HandleTypeDef _handle{};
        IndependentWatchDogConfig _config{};

        /// @brief 内部时钟信号的频率。还要经过预分频才会输入到计数器。
        /// @return
        base::Hz InnerClockSourceFreq() const
        {
            // 独立看门狗具有 40 kHz 的内部时钟。
            return base::Hz{40 * 1000};
        }

    public:
        static_function IndependentWatchDog &Instance()
        {
            class Getter :
                public base::SingletonGetter<IndependentWatchDog>
            {
            public:
                std::unique_ptr<IndependentWatchDog> Create() override
                {
                    return std::unique_ptr<IndependentWatchDog>{new IndependentWatchDog{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }

        IWDG_TypeDef *HardwareInstance()
        {
            return IWDG1;
        }

        /// @brief 打开看门狗定时器。
        /// @param timeout 看门狗超时时间。
        void Open(std::chrono::milliseconds value) override;

        /// @brief 关闭看门狗定时器。
        void Close()
        {
        }

        /// @brief 看门狗超时时间。
        /// @return
        std::chrono::milliseconds Timeout() const override;

        void Feed() override;
    };
} // namespace hal
