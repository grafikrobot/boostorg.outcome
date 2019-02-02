/* Proposed SG14 status_code
(C) 2018-2019 Niall Douglas <http://www.nedproductions.biz/> (5 commits)
File Created: June 2018


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

#ifndef BOOST_OUTCOME_SYSTEM_ERROR2_SYSTEM_CODE_FROM_EXCEPTION_HPP
#define BOOST_OUTCOME_SYSTEM_ERROR2_SYSTEM_CODE_FROM_EXCEPTION_HPP

#include "system_code.hpp"

#include <exception>  // for exception_ptr
#include <stdexcept>  // for the exception types

BOOST_OUTCOME_SYSTEM_ERROR2_NAMESPACE_BEGIN

/*! A utility function which returns the closest matching system_code to a supplied
exception ptr.
*/
inline system_code system_code_from_exception(std::exception_ptr &&ep = std::current_exception(), system_code not_matched = generic_code(errc::resource_unavailable_try_again)) noexcept
{
  if(!ep)
  {
    return generic_code(errc::success);
  }
  try
  {
    std::rethrow_exception(ep);
  }
  catch(const std::invalid_argument & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::invalid_argument);
  }
  catch(const std::domain_error & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::argument_out_of_domain);
  }
  catch(const std::length_error & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::argument_list_too_long);
  }
  catch(const std::out_of_range & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::result_out_of_range);
  }
  catch(const std::logic_error & /*unused*/) /* base class for this group */
  {
    ep = std::exception_ptr();
    return generic_code(errc::invalid_argument);
  }
  catch(const std::system_error &e) /* also catches ios::failure */
  {
    ep = std::exception_ptr();
    if(e.code().category() == std::generic_category())
    {
      return generic_code(static_cast<errc>(static_cast<int>(e.code().value())));
    }
    if(e.code().category() == std::system_category())
    {
#ifdef _WIN32
      return win32_code(e.code().value());
#else
      return posix_code(e.code().value());
#endif
    }
    // Don't know this error code category, can't wrap it into std_error_code
    // as its payload won't fit into system_code, so fall through.
  }
  catch(const std::overflow_error & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::value_too_large);
  }
  catch(const std::range_error & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::result_out_of_range);
  }
  catch(const std::runtime_error & /*unused*/) /* base class for this group */
  {
    ep = std::exception_ptr();
    return generic_code(errc::resource_unavailable_try_again);
  }
  catch(const std::bad_alloc & /*unused*/)
  {
    ep = std::exception_ptr();
    return generic_code(errc::not_enough_memory);
  }
  catch(...)
  {
  }
  return not_matched;
}

BOOST_OUTCOME_SYSTEM_ERROR2_NAMESPACE_END

#endif
