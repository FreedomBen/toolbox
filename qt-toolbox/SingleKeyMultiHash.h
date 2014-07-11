
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

#ifndef __SINGLE_KEY_MULTI_HASH_H__
#define __SINGLE_KEY_MULTI_HASH_H__

#include <QHash>
#include <QList>


template <class Key, class T>
class SingleKeyMultiHash
{
    public:

        SingleKeyMultiHash( bool duplicateValuesAllowed = true );
        ~SingleKeyMultiHash();

        SingleKeyMultiHash( const SingleKeyMultiHash &other );
        SingleKeyMultiHash & operator=( const SingleKeyMultiHash &other );

        void insert( const Key & key, const T & value );
        void insertMulti( const Key & key, const T & value );

        QList<Key> keys() const;

        const QList<T> values( const Key & key );
        const QList<T> values() const;
        const QList<T> values( const Key & key ) const;

        bool contains( const Key & key ) const;
        bool contains( const Key & key, const T & value ) const;

        int	count() const;
        int	count( const Key & key ) const;
        int	count( const Key & key, const T & value ) const;

        int	remove( const Key & key );
        int	remove( const Key & key, const T & value );

        void clear();
        void clear( const Key & key );

    private:

        void _initKey( const Key & key ) const;
        int _removeKey( const Key & key );
        bool _hasDuplicates( const Key & key ) const;

        mutable QHash<Key, QList<T> * > _hash;
        bool _duplicateValuesAllowed;
};


#include "SingleKeyMultiHash.cxx"


#endif // __SINGLE_KEY_MULTI_HASH_H__
