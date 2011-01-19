/* 
 * File:   IdTooLargeException.cc
 * Author: blueck
 * 
 * Created on 18. Januar 2011, 11:35
 */

#include "IdTooLargeException.h"

IdTooLargeException::IdTooLargeException() { }

const char* IdTooLargeException::what() const throw()
{
    return "The id supplied is out of range, or there are already too many ids.";
}


