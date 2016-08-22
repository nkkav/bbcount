/* file "bbcount/bbcount.h" */
/*
 *     Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 *                   2011, 2012, 2013, 2014, 2015, 2016 Nikolaos Kavvadias
 *
 *     This software was written by Nikolaos Kavvadias, Ph.D. candidate
 *     at the Physics Department, Aristotle University of Thessaloniki,
 *     Greece (at the time).
 *
 *     This software is provided under the terms described in
 *     the "machine/copyright.h" include file.
 */

#ifndef BBCOUNT_BBCOUNT_H
#define BBCOUNT_BBCOUNT_H

#include <machine/copyright.h>

#ifdef USE_PRAGMA_INTERFACE
#pragma interface "bbcount/bbcount.h"
#endif

#include <machine/machine.h>

class Bbcount {
  public:
    Bbcount() { }

    void initialize() { }
    void do_opt_unit(OptUnit*);
    void finalize() { }
};

#endif /* BBCOUNT_BBCOUNT_H */
