#ifndef __CALLBACKS_H_
#define __CALLBACKS_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <utils/Timestamp.h>

namespace dbdky
{
    template<typename To, typename From>
    inline ::boost::shared_ptr<To> down_pointer_cast(const ::boost::shared_ptr<From>& f)
    {
        if (false)
        {
            implicit_cast<From*, To*>(0);
        }
#ifndef NDEBUG
        assert(f == NULL || dynamic_cast<To*>(get_pointer(f)) != NULL);
#endif

        return ::boost::static_pointer_cast<To>(f);
    }


    namespace port
    {
        class Buffer;
        class ComEntity;
        typedef boost::shared_ptr<ComEntity> ComEntityPtr;
        typedef boost::function<void()> TimerCallback;
        typedef boost::function<void(const ComEntityPtr&)> ConnectionCallback;
        typedef boost::function<void(const ComEntityPtr&)> CloseCallback;
        typedef boost::function<void(const ComEntityPtr&)> WriteCompleteCallback;
        //typedef boost::function<void<const ComEntityPtr&, size_t)> HighWaterMardCallback;

        typedef boost::function<void(const ComEntityPtr&,
				Buffer*,
				Timestamp)> MessageCallback;

        void defaultConnectionCallback(const ComEntityPtr& conn);
        void defaultMessageCallback(const ComEntityPtr& conn,
			Buffer* buffer,
			Timestamp receiveTime);
    }
}


#endif
