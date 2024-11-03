#include "SysTickClock.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/interrupt.h>

extern "C"
{
    /// @brief 重写 __weak 的 HAL_Delay 函数
    /// @param ms 要延时的毫秒数。
    void HAL_Delay(uint32_t ms)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{ms});
    }
}

hal::SysTickClock &hal::SysTickClock::Instance()
{
    class Getter :
        public base::SingletonGetter<SysTickClock>
    {
    public:
        std::unique_ptr<SysTickClock> Create() override
        {
            return std::unique_ptr<SysTickClock>{new SysTickClock{}};
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

uint32_t hal::SysTickClock::Frequency() const
{
    // stm32h743 不支持 8 分频。
    uint32_t freq = HAL_RCC_GetSysClockFreq();
    return freq;
}

uint32_t hal::SysTickClock::ReloadValue() const
{
    uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
    return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t hal::SysTickClock::CurrentValue() const
{
    uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
    return masked >> SysTick_VAL_CURRENT_Pos;
}
