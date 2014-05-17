
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
