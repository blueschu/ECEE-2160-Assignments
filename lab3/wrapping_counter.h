/**
 * ECEE 2160 Lab Assignment 3.4 counter helper utilitiy.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-26
 *
 * References
 * ==========
 *
 * - https://en.cppreference.com/w/cpp/language/operator_incdec
 *
 */

#ifndef ECEE_2160_LAB_REPORTS_WRAPPING_COUNTER_H
#define ECEE_2160_LAB_REPORTS_WRAPPING_COUNTER_H

#include <functional>       // for std::function

/**
 * A counter over an interval [0,max] for some max that wraps around its
 * endpoints.
 */
class WrappingCounter {
    /**
     * Integral type used to represent this counter's value.
     */
    using Count = std::uint64_t;

    /**
     * The current state of the counter.
     */
    Count m_couter{0};

    /**
     * The maximum value that the counter can reached before wrapping to 0.
     */
    const Count m_max;

  public:
    /**
     * Constructs a counter that wraps upon advancing past the given maximum.
     *
     * @param max Maximum counter value.
     */
    explicit WrappingCounter(Count max) : m_max{max} {}

    /**
     * Converts this counter into an integral value.
     *
     * We allow this conversion to occur implicitly for simplicity.
     *
     * @return Counter state.
     */
    operator Count() const { return m_couter; }

    /**
     * Applies the given callable to the internal counter and stores the
     * result, modulo (this counter max value + 1), as the new counter value.
     *
     * @param func Callable to mutate the internal counter.
     */
    void apply(std::function<Count(Count)> func)
    {
        auto new_counter = func(m_couter);
        m_couter = new_counter % (m_max + 1);
    }

    Count operator++()
    {
        m_couter = m_couter == m_max ? 0 : m_couter + 1;
        return m_couter;
    }

    Count operator--()
    {
        m_couter = m_couter == 0 ? m_max : m_couter - 1;
        return m_couter;
    }

    Count operator++(int)
    {
        auto tmp = m_couter;
        ++(*this);
        return tmp;
    }

    Count operator--(int)
    {
        auto tmp = m_couter;
        --(*this);
        return tmp;
    }
};

#endif //ECEE_2160_LAB_REPORTS_WRAPPING_COUNTER_H
