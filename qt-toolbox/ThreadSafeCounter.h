
/**
 *  Copyright (c) 2014, Benjamin Porter
 *  All rights reserved.
 * 
 *  (BSD style license)
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 *  1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THREADSAFECOUNTER_H
#define THREADSAFECOUNTER_H

#include <QSemaphore>
#include <log/QtLogger.h>

/*******************************************************
 * Provides a simple thread safe counter
 *
 * Is basically a thin wrapper over a QSemaphore
 *
 * Don't try to go negative or positive above the limits
 *******************************************************/

class ThreadSafeCounter : private QSemaphore
{
    public:

        static const qint32 LowerLimit     = -2000000; // lowest number to be counted to
        static const qint32 UpperLimit     =  2000000; // highest number to be counted to
        static const qint32 SemaphoreLimit =  4000000; // absolute value of LowerLimit + absolute value of UpperLimit
        static const qint32 Zero           =  2000000; // half of the semaphore limit

        ThreadSafeCounter( qint32 initialValue = 0 ) :
            QSemaphore( SemaphoreLimit ),
            _zero( 0 )
        {
            Q_ASSERT( count() == 0 );
            Q_ASSERT( ( SemaphoreLimit - available() ) == 0 );

            increment( Zero );
            _zero = Zero;

            Q_ASSERT( count() == 0 );

            if( initialValue > 0 )
                increment( initialValue );
            else if( initialValue < 0 )
                decrement( initialValue );

            Q_ASSERT( count() == initialValue );
        }

        qint32 count()
        {
            return SemaphoreLimit - available() - _zero;
        }

        void increment( qint32 n = 1 )
        {
            qDebug() << __CLASS_FUNCTION__ << "Count-" << count();
            Q_ASSERT( count() < UpperLimit );

            acquire( n );
        }

        void decrement( qint32 n = 1 )
        {
            Q_ASSERT( count() > LowerLimit );

            release( n );
        }

    private:

        qint32 _zero;
};

#endif // THREADSAFECOUNTER_H
