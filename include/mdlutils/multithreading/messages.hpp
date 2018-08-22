//
// Created by marandil on 09.09.15.
//

#ifndef MDLUTILS_MULTITHREADING_MESSAGES_HPP
#define MDLUTILS_MULTITHREADING_MESSAGES_HPP

#include <memory>
#include <functional>

#include <mdlutils/multithreading/helpers.hpp>

namespace mdl
{
    /* Base structure for messages handled by the <looper>s. */
    struct message
    {
        virtual ~message() { }
    };

    /* Message used to force loopers to break out their loops. */
    struct break_message : public message
    { virtual ~break_message() {} };

    /* Message used by <executor_handler> et.al. to call functions passed along with the message. */
    struct post_call : public message
    {
        /* Create a post_call message for a given function. */
        post_call(std::function<void(void)> &&function) : function(std::move(function)) { }

        virtual ~post_call() { }
    
        // The function carried along with the message.
        std::function<void(void)> function;
    };

    /* Message used by <delaying_handler> et.al. to requeue the enveloped messages until a time point is reached. */
    struct delayed_message : public message
    {
        /* Create a delayed_message.
         * @content A message to be delayed.
         * @delayed_until A time point after which the message will be requeued without the envelope.
         */
        delayed_message(std::shared_ptr<message> content,
                        time_point_t delayed_until) : content(content), delayed_until(delayed_until) { }

        virtual ~delayed_message() { }

        // The internal (enveloped) message.
        std::shared_ptr<message> content;
        // Time point after which the content will be requeued without the envelope.
        time_point_t delayed_until;
    };

    /* Shared pointer to the message type. All message queues and handlers should use this type. */
    typedef std::shared_ptr<message> message_ptr;
}

#endif //MDLUTILS_MULTITHREADING_MESSAGES_HPP
