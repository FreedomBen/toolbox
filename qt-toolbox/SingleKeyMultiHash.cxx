
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

#include <QSet>

#include "log/QtLogger.h"

#include "SingleKeyMultiHash.h"

template <class Key, class T>
SingleKeyMultiHash<Key, T>::SingleKeyMultiHash( bool duplicateValuesAllowed ) :
    _duplicateValuesAllowed( duplicateValuesAllowed )
{  }

template <class Key, class T>
SingleKeyMultiHash<Key, T>::SingleKeyMultiHash( const SingleKeyMultiHash &other )
{
    // we need new pointers allocated for our QLists
    foreach( const Key &key, other.keys() )
    {
        _initKey( key );

        foreach( const T &t, other.values( key ) )
            _hash.value( key )->append( t );
    }
}

template <class Key, class T>
SingleKeyMultiHash<Key, T> & SingleKeyMultiHash<Key, T>::operator=( const SingleKeyMultiHash<Key, T> &other )
{
    if( this != &other ) // check for self-assignment
    {
        // we need new pointers allocated for our QLists
        foreach( const Key &key, _hash.keys() )
        {
            if( _hash.value( key ) )
                delete _hash.value( key );
        }

        _hash.clear();

        foreach( const Key &key, other.keys() )
        {
            _initKey( key );

            foreach( const T &t, other.values( key ) )
                _hash.value( key )->append( t );
        }
    }
    return *this;
}

template <class Key, class T>
SingleKeyMultiHash<Key, T>::~SingleKeyMultiHash()
{
    foreach( QList<T> * list, _hash.values() )
        delete list;
}

template <class Key, class T>
void SingleKeyMultiHash<Key, T>::insert( const Key & key, const T & value )
{
#ifndef QT_NO_DEBUG
    if( !_duplicateValuesAllowed && _hash.value( key ) && _hash.value( key )->contains( value ) )
        qWarning() << __CLASS_FUNCTION__ << ": Trying to add duplicate value when duplicates are disallowed";
#endif

    _initKey( key );
    if( _duplicateValuesAllowed || !_hash.value( key )->contains( value ) )
    {
        Q_ASSERT( _duplicateValuesAllowed ? true : !_hash.value( key )->contains( value ) );
        _hash.value( key )->append( value );
        Q_ASSERT( _hash.value( key )->contains( value ) );
    }

    Q_ASSERT( contains( key ) );
    Q_ASSERT( contains( key, value ) );
    Q_ASSERT( _duplicateValuesAllowed ? true : !_hasDuplicates( key ) );
}

template <class Key, class T>
void SingleKeyMultiHash<Key, T>::insertMulti( const Key & key, const T & value )
{
    insert( key, value );
}

template <class Key, class T>
const QList<T> SingleKeyMultiHash<Key, T>::values() const
{
    QList<T> retval;

    foreach( QList<T> *l, _hash.values() )
        retval += *l;

    return retval;
}

template <class Key, class T>
const QList<T> SingleKeyMultiHash<Key, T>::values( const Key & key )
{
    if( _hash.value( key ) )
        return *_hash.value( key );
    else
        return QList<T>();
}

template <class Key, class T>
const QList<T> SingleKeyMultiHash<Key, T>::values( const Key & key ) const
{
    if( _hash.value( key ) )
        return *_hash.value( key );
    else
        return QList<T>();
}

template <class Key, class T>
bool SingleKeyMultiHash<Key, T>::contains( const Key & key ) const
{
    return _hash.contains( key );
}

template <class Key, class T>
bool SingleKeyMultiHash<Key, T>::contains( const Key & key, const T & value ) const
{
    if( _hash.contains( key ) )
        return _hash.value( key )->contains( value );
    else
        return false;
}

template <class Key, class T>
int	SingleKeyMultiHash<Key, T>::count() const
{
    return _hash.count();
}

template <class Key, class T>
int	SingleKeyMultiHash<Key, T>::count( const Key & key ) const
{
    if( _hash.value( key ) )
        return _hash.value( key )->count();
    else
        return 0;
}

template <class Key, class T>
int	SingleKeyMultiHash<Key, T>::count( const Key & key, const T & value ) const
{
    if( _hash.value( key ) )
        return _hash.value( key )->count( value );
    else
        return 0;
}

template <class Key, class T>
int	SingleKeyMultiHash<Key, T>::remove( const Key & key )
{
    return _removeKey( key );
}

template <class Key, class T>
int	SingleKeyMultiHash<Key, T>::remove( const Key & key, const T & value )
{
    int itemsRemoved = 0;

    if( _hash.contains( key ) )
    {
        Q_ASSERT( _hash.value( key ) != NULL );

        if( _hash.value( key ) )
        {
            itemsRemoved = _hash.value( key )->removeAll( value );
            Q_ASSERT( !contains( key, value ) );

            if( _hash.value( key )->isEmpty() )
            {
#ifndef QT_NO_DEBUG
                int preCount = _hash.count();
                Q_ASSERT( preCount == _hash.keys().count() );
                Q_ASSERT( preCount > 0 );
#endif
                Q_ASSERT( _hash.value( key )->count() == 0 );
                Q_ASSERT( contains( key ) );
                Q_ASSERT( _hash.contains( key ) );
                Q_ASSERT( _hash.keys().contains( key ) );
                _removeKey( key );
                Q_ASSERT( !contains( key ) );
                Q_ASSERT( !_hash.contains( key ) );
                Q_ASSERT( !_hash.keys().contains( key ) );
                Q_ASSERT( _hash.count() <= preCount - 1 );
                Q_ASSERT( _hash.count() == _hash.keys().count() );
            }
            else
            {
                Q_ASSERT( _hash.value( key )->count() > 0 );
            }
        }
    }

    Q_ASSERT( !contains( key, value ) );

    return itemsRemoved;
}

template <class Key, class T>
void SingleKeyMultiHash<Key, T>::_initKey( const Key & key ) const
{
    if( !_hash.contains( key ) )
    {
        _hash.insert( key, new QList<T>() );
    }

    Q_ASSERT( _hash.contains( key ) );
    Q_ASSERT( _hash.value( key ) != NULL );
}

template <class Key, class T>
void SingleKeyMultiHash<Key, T>::clear()
{
    _hash.clear();
}

template <class Key, class T>
void SingleKeyMultiHash<Key, T>::clear( const Key & key )
{
    _removeKey( key );
}

template <class Key, class T>
int SingleKeyMultiHash<Key, T>::_removeKey( const Key & key )
{
    if( _hash.contains( key ) )
    {
        QList<T> *val = _hash.value( key );
        int retval = 0 + val->count();

        if( val )
            delete val;

#ifndef QT_NO_DEBUG
        int prevCount = count();
        int prevKeyCount = keys().count();

        Q_ASSERT( keys().count() == count() );
#endif

        _hash.remove( key );

        Q_ASSERT( count() == ( prevCount - 1 ) );
        Q_ASSERT( keys().count() == ( prevKeyCount - 1 ) );
        Q_ASSERT( keys().count() == count() );

        Q_ASSERT( _hash.value( key ) == NULL );
        Q_ASSERT( !_hash.contains( key ) );
        Q_ASSERT( !contains( key ) );

        return retval;
    }
    else
    {
        return 0;
    }
}

template <class Key, class T>
QList<Key> SingleKeyMultiHash<Key, T>::keys() const
{
    return _hash.keys();
}

template <class Key, class T>
bool SingleKeyMultiHash<Key, T>::_hasDuplicates( const Key & key ) const
{
    QSet<T> values;

    foreach( const T &val, (*_hash.value( key ) ) )
    {
        if( values.contains( val ) )
            return true;
        else
            values.insert( val );
    }
    return false;
}
