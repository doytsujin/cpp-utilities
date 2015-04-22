#include "conversionexception.h"

namespace ConversionUtilities {

/*!
 * \class ConversionUtilities::ConversionException
 * \brief The exception that is thrown when an conversion error occurs.
 */

/*!
 * Constructs a new ConversionException.
 */
ConversionException::ConversionException() USE_NOTHROW :
    runtime_error("unable to convert")
{}

/*!
 * Constructs a new ConversionException. \a what is a std::string
 * describing the cause of the ConversionException.
 */
ConversionException::ConversionException(const std::string &what) USE_NOTHROW :
    runtime_error(what)
{}

/*!
 * Destroys the ConversionException.
 */
ConversionException::~ConversionException() USE_NOTHROW
{}

}
