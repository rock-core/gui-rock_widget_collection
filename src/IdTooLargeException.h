/* 
 * File:   IdTooLargeException.h
 * Author: blueck
 *
 * Created on 18. Januar 2011, 11:35
 */

#ifndef IDTOOLARGEEXCEPTION_H
#define	IDTOOLARGEEXCEPTION_H

#include <iostream>
#include <typeinfo>

class IdTooLargeException : public std::exception
{
public:
    IdTooLargeException();
    virtual const char* what() const throw();
private:

};

#endif	/* IDTOOLARGEEXCEPTION_H */

