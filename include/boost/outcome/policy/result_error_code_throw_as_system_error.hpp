/* Policies for result and outcome
(C) 2017 Niall Douglas <http://www.nedproductions.biz/> (59 commits)
File Created: Oct 2017


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_OUTCOME_POLICY_RESULT_ERROR_CODE_THROW_AS_SYSTEM_ERROR_HPP
#define BOOST_OUTCOME_POLICY_RESULT_ERROR_CODE_THROW_AS_SYSTEM_ERROR_HPP

#include "../bad_access.hpp"
#include "detail/common.hpp"

#include <system_error>

BOOST_OUTCOME_V2_NAMESPACE_EXPORT_BEGIN

namespace policy
{
  template <class T, class EC, class E> struct error_code_throw_as_system_error;
  /*! Policy interpreting `EC` as a type for which `trait::has_error_code_v<EC>` is true.
  Any wide attempt to access the successful state where there is none causes:

  1. If `trait::has_error_payload_v<EC>` is true, it calls an
  ADL discovered free function `throw_as_system_error_with_payload(.error())`.
  2. If `trait::has_error_payload_v<EC>` is false, it calls `BOOST_OUTCOME_THROW_EXCEPTION(std::system_error(policy::error_code(.error())))`
  */
  template <class T, class EC> struct error_code_throw_as_system_error<T, EC, void> : detail::base
  {
    /*! Performs a wide check of state, used in the value() functions.
    \effects See description of class for effects.
    */
    template <class Impl> static constexpr void wide_value_check(Impl &&self)
    {
      if((self._state._status & BOOST_OUTCOME_V2_NAMESPACE::detail::status_have_value) == 0)
      {
        if((self._state._status & BOOST_OUTCOME_V2_NAMESPACE::detail::status_have_error) != 0)
        {
          // ADL discovered
          throw_as_system_error_with_payload(std::forward<Impl>(self)._error);
        }
        BOOST_OUTCOME_THROW_EXCEPTION(bad_result_access("no value"));
      }
    }
    /*! Performs a wide check of state, used in the error() functions
    \effects If result does not have an error, it throws `bad_result_access`.
    */
    template <class Impl> static constexpr void wide_error_check(Impl &&self)
    {
      if((self._state._status & BOOST_OUTCOME_V2_NAMESPACE::detail::status_have_error) == 0)
      {
        BOOST_OUTCOME_THROW_EXCEPTION(bad_result_access("no error"));
      }
    }
  };
}  // namespace policy

BOOST_OUTCOME_V2_NAMESPACE_END

#endif
