#pragma once

#include <boost/thread.hpp>

#include "Disruptor/IWaitStrategy.h"


namespace Disruptor
{

    /**
     * <p>The SpinCountBackoffWait strategy spins for a given number of times then
     * waits using the configured fallback WaitStrategy.</p>
     * This combination wait strategy worked well in Java with the fallback being BlockingWaitStrategy,
     * so that's the default here.
     *
     * @author timmer
     * @date 2/18/2020
     */
    class SpinCountBackoffWaitStrategy : public IWaitStrategy
    {

    public:

        /** Constructor defaulting to 10,000 spins and BlockingWaitStrategy when finished spinning. */
        SpinCountBackoffWaitStrategy();

        /**
         * Constructor specifying number of spins and the back up wait strategy it switches to after spinning.
         * @param spinTries number of spins.
         * @param fallbackStrategy back up wait strategy.
         */
        SpinCountBackoffWaitStrategy(std::uint32_t spinTries, std::shared_ptr<IWaitStrategy> fallbackStrategy);

        /**
         * \see IWaitStrategy::waitFor
         */
        std::int64_t waitFor(std::int64_t sequence,
                             Sequence& cursor,
                             ISequence& dependentSequence,
                             ISequenceBarrier& barrier) override;

        /**
         * \see IWaitStrategy::signalAllWhenBlocking
         */
        void signalAllWhenBlocking() override;

        void writeDescriptionTo(std::ostream& stream) const override;

    private:

        std::uint32_t SPIN_TRIES;
        std::shared_ptr<IWaitStrategy> fallbackStrategy;

    };

} // namespace Disruptor
